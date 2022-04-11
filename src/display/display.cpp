#include "display.h"
#include "frame.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <cassert>

using LED::Display;
using LED::Frame;

const std::string GPIO_BASE_PATH{"/sys/class/gpio/"};
constexpr const long DELAY_IN_US = 5;

enum PIN_INDEX {
    R1 = 0, // top half
    G1,
    B1,

    R2,     // bottom half
    G2,
    B2,

    CLK,    // clock
    LAT,    // latch/strobe
    OE,     // on/off

    A,      // setting row
    B,
    C
};

#define NUM_PINS 12
constexpr int GPIO_PINS[NUM_PINS] = {8, 80, 78, 76, 79, 74, 73, 75, 71, 72, 77, 70};
std::fstream GPIO_FILES[NUM_PINS];

Display::Display(int num_chains,
                 int num_stack,
                 bool serpentine
                ):  NUM_CHAIN{num_chains},
                    NUM_STACK{num_stack},
                    SERPENTINE{serpentine},
                    NUM_PANELS{ num_chains * num_stack},
                    framebuffer( PANEL_ROWS , std::vector<int>(PANEL_COLUMNS * NUM_PANELS, BLACK)),
                    running{true},
                    update{true},
                    refresh_thread{&Display::refresh, this} { }

Display::~Display(){
    // stop the thread
    running = false;
    refresh_thread.join();

    // close the gpio files
    for(auto& file: GPIO_FILES){
        file.close();
    }
}

void Display::exportPins(){

    std::string path{ GPIO_BASE_PATH + "export" };
    std::ofstream f_export(path, std::ofstream::out | std::ofstream::trunc );

    for(int pin : GPIO_PINS){
        f_export << pin << std::endl;
    }
    f_export.close();

    std::this_thread::sleep_for( std::chrono::milliseconds(300) );
    return;
}

void Display::setPinDirection(){

    std::ofstream f_direction;
    for(int pin : GPIO_PINS){
        std::string path{ GPIO_BASE_PATH + "gpio" + std::to_string(pin) + "/direction" };

        f_direction.clear();
        f_direction.open(path, std::ofstream::out | std::ofstream::trunc );
        f_direction << "out" << std::endl;
        f_direction.close();
    }
    return;
}

void Display::setGPIOFiles(){

    for(int pin = R1; pin <= C; ++pin){
        std::string path{ GPIO_BASE_PATH + "gpio" + std::to_string( GPIO_PINS[ pin ] ) + "/value" };
        GPIO_FILES[ pin ].open( path, std::ofstream::out | std::ofstream::trunc );
    }
    return;
}

void Display::setRow( int row ) {
    std::bitset<3> r = row;
    GPIO_FILES[ A ].seekp(0);
    GPIO_FILES[ A ] << r.test(0) << std::endl;// iiii & 0100

    GPIO_FILES[ B ].seekp(0);
    GPIO_FILES[ B ] << r.test(1) << std::endl;

    GPIO_FILES[ C ].seekp(0);
    GPIO_FILES[ C ] << r.test(2) << std::endl;
}

void Display::setTop( int color ) {
    std::bitset<3> c = color;

    GPIO_FILES[ R1 ].seekp(0);
    GPIO_FILES[ R1 ] << c.test(0) << std::endl;

    GPIO_FILES[ G1 ].seekp(0);
    GPIO_FILES[ G1 ] << c.test(1) << std::endl;

    GPIO_FILES[ B1 ].seekp(0);
    GPIO_FILES[ B1 ] << c.test(2) << std::endl;

}

void Display::setBtm( int color ) {
    std::bitset<3> c = color;

    GPIO_FILES[ R2 ].seekp(0);
    GPIO_FILES[ R2 ] << c.test(0) << std::endl;

    GPIO_FILES[ G2 ].seekp(0);
    GPIO_FILES[ G2 ] << c.test(1) << std::endl;

    GPIO_FILES[ B2 ].seekp(0);
    GPIO_FILES[ B2 ] << c.test(2) << std::endl;
}

void Display::clock() {
    GPIO_FILES[ CLK ].seekp(0);
    GPIO_FILES[ CLK ] << 1<< std::endl;
    GPIO_FILES[ CLK ].seekp(0);
    GPIO_FILES[ CLK ] << 0<< std::endl;
}

void Display::lat(){
    GPIO_FILES[ LAT ].seekp(0);
    GPIO_FILES[ LAT ] << 1<< std::endl;

    GPIO_FILES[ LAT ].seekp(0);
    GPIO_FILES[ LAT ] << 0<< std::endl;
}

void Display::on() {
    GPIO_FILES[ OE ].seekp(0);
    GPIO_FILES[ OE ] << 0<< std::endl;
}

void Display::off() {
    GPIO_FILES[ OE ].seekp(0);
    GPIO_FILES[ OE ] << 1<< std::endl;
}

void Display::updateFrame( Frame frame ){
    injestFrame( frame.frameBuffer );
}

void Display::updateFrame( std::vector<std::vector<int>>& frame ) {
    assert( frame.size() == 32);
    assert( frame.begin()->size() == 32);
    injestFrame( frame );
}

void Display::injestFrame( const std::vector<std::vector<int>>& newFrame ) {
    update_mutex.lock();

    // get upper part the frame
    std::vector<std::vector<int>> upper;
    upper.assign( newFrame.begin(), newFrame.begin() + PANEL_ROWS);

    // flip vertically
    std::reverse(upper.begin(), upper.end());

    // flip horizontally
    std::for_each(
        upper.begin(),
        upper.end(),
        [](auto& row){
            std::reverse( row.begin(), row.end() );
        }
    );

    framebuffer.clear();

    framebuffer.reserve( PANEL_ROWS );
    framebuffer.insert( framebuffer.begin(), upper.begin(), upper.end() );

    auto lowerRow = newFrame.begin() + PANEL_ROWS;
    for( auto& row : framebuffer ) {
        row.insert( row.end(), lowerRow->begin(), lowerRow->end() );
        ++lowerRow;
    }

    update = true;
    update_mutex.unlock();
}

void Display::refresh(){
    exportPins();
    setPinDirection();
    setGPIOFiles();

    // local copy of an array to push out to pins
    std::vector<std::vector<int>> local_buffer;

    while(running){
        // try lock to update local from class array
        if( update && update_mutex.try_lock() ){
            local_buffer.clear();
            local_buffer.assign( framebuffer.begin(), framebuffer.end());
            update = false;
            update_mutex.unlock();
        }
        // std::cout << local_buffer.size() << "\t" << local_buffer.begin()->size() << std::endl;

        for( int i = 0; i < PANEL_ROWS/2; ++i ){ // cannot do foreach cuz looping only half rows
            // diable matrix
            off();
            // set row
            // a b c // TODO: using std::bitset instead of comparison
            setRow(i);
            // loop over column and send the data
                for(int col = 0; col < 64; col++){
                    // upper, r1, g1, b1
                    setTop( local_buffer[i][col]);
                    // lower, r2, g2, b2
                    setBtm( local_buffer[i+8][col]);
                    // toggle clock
                    clock();
                } // for coloum
            // toggle latch
            lat();
            // enable display
            on();

            std::this_thread::sleep_for( std::chrono::nanoseconds( DELAY_IN_US ) );
        } // for row
    } // while
}
