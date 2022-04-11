#pragma once

#include "frame.h"
#include <vector>
#include <thread>
#include <mutex>

namespace LED {
    class Frame;
    enum class Colour: int;

    class Display {
        public:
            /**
             * @brief Construct a new Display object
             *
             * @param num_chains # of panels chained across
             * @param num_stack # of panels stacked
             * @param serpentine If true every other display is rotated 180, relevant only for stacked configs
             */
            Display(int num_chains, int num_stack, bool serpentine);
            ~Display();

            void updateFrame( Frame frame);
            void updateFrame( std::vector<std::vector<int>>& frame);

        private:
            /*
                - PANEL SIZE:
                    - num rows
                    - num cols

                - num chain
                - num stack
                - serpentine
            */
            bool running = false;
            const int PANEL_ROWS = 16;
            const int PANEL_COLUMNS = 32;

            int NUM_CHAIN; // number of panels chained across
            int NUM_STACK; // number of panels tiled down
            bool SERPENTINE;
            int NUM_PANELS;

            std::thread refresh_thread;
            std::mutex update_mutex;

            void exportPins();
            void setPinDirection();
            void setGPIOFiles();

            void setRow(int row);
            void setTop(int color);
            void setBtm(int color);

            void clock();
            void lat();

            void on();
            void off();

            void injestFrame(const std::vector<std::vector<int>>& frame);
            void refresh();

            // The current frame that refresh thread will copy to a local copy
            std::vector< std::vector<int> > framebuffer;
            bool update = false;

    };

} // LED namespace
