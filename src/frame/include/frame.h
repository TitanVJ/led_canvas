#pragma once
#include <Eigen/Dense>
#include <vector>
#include "display.h"

namespace LED {

    enum Colour {
        BLACK = 0,
        RED,
        GREEN,
        BLUE,
        CYAN,
        MAGENTA,
        YELLOW,
        WHITE
    };

    class Frame {
        public:
            Frame();
            ~Frame();

            void at(int row, int column, Colour colour);
            bool set(int row, int column, Colour colour);

            // set row/col
            bool setRow(int row, std::vector<Colour> row_data);
        private:
            // internal matrix

            // function to return reshaped and rotated matrix/array
            friend class LED::Display;
    };

} // LED namespace