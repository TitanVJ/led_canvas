#include "frame.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

using LED::Frame;

// todo: ppa file constructor

void Frame::at(size_t row, size_t col, int colour) {
    assert( row < HEIGHT);
    assert( col < WIDTH);

    frameBuffer[row][col] = colour;
}

void Frame::shiftUp(size_t shift_by) {
    // verify shift value
    if(shift_by >= HEIGHT){
        shift_by = HEIGHT;

    }

    auto row = std::rotate( frameBuffer.begin(),
                            frameBuffer.begin() + shift_by,
                            frameBuffer.end()
                            );
    // check: this is working as intended
    std::fill(row, frameBuffer.end(), std::vector<int>(WIDTH, BLACK));

    // while(row != frameBuffer.end()){
    //     *row = std::vector{WIDTH, BLACK};
    //     ++row;
    // }
}

void Frame::shiftDown(size_t shift_by) {
    // verify shift value
    if(shift_by >= HEIGHT){
        shift_by = HEIGHT;
    }

    auto row = std::rotate( frameBuffer.rbegin(),
                            frameBuffer.rbegin() + shift_by,
                            frameBuffer.rend()
                            );

    std::fill(row, frameBuffer.rend(), std::vector<int>(WIDTH, BLACK));
}

void Frame::shiftLeft(size_t shift_by) {
    std::for_each(  frameBuffer.begin(),
                    frameBuffer.end(),
                    [shift_by](auto& row){
                        auto start = std::rotate(
                                        row.begin(),
                                        row.begin() + shift_by,
                                        row.end()
                                    );
                        std::fill(
                            start,
                            row.end(),
                            BLACK
                        );
                    }
                );
}

void Frame::shiftRight(size_t shift_by) {
    std::for_each(  frameBuffer.begin(),
                    frameBuffer.end(),
                    [shift_by](auto& row){
                        auto start = std::rotate(
                                        row.rbegin(),
                                        row.rbegin() + shift_by,
                                        row.rend()
                                    );
                        std::fill(
                            start,
                            row.rend(),
                            BLACK
                        );
                    } // lambda
                );
}

void Frame::print() {
    for( auto row : frameBuffer ){
        for( auto col: row ){
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return;
}

std::ostream& LED::operator<<(std::ostream& os, const Frame& f){

    for( auto row : f.frameBuffer ){
        for( auto col: row ){
            os << col << " ";
        }
        os << "\n";
    }
        return os;
}
