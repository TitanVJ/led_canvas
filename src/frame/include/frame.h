#pragma once
#include "display.h"
#include <ostream>
#include <vector>

#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define CYAN 4
#define MAGENTA 5
#define YELLOW 6
#define WHITE 7

namespace LED {
    class Display;

    class Frame {
        public:

            // default frame of all black
            Frame(): frameBuffer{HEIGHT, std::vector<int>{WIDTH, BLACK}} { }
            // initialize a frame with all the same `colour` values
            Frame(int colour): frameBuffer{HEIGHT, std::vector<int>{WIDTH, colour}} { }
            // construct a frame from an existing 2d vector of ints
            Frame(const std::vector<std::vector<int>>& frame_data) { frameBuffer.assign( frame_data.begin(), frame_data.end() ); };
            // Frame(std::string_view file_path);

            // copy constructor
            Frame(const Frame& frame): frameBuffer{ frame.frameBuffer } { }

            ~Frame() = default;

            // sets pixel to `colour` at the given `row` and `colomn`
            void at(size_t row, size_t column, int colour);
            // bool set(int row, int column, Colour colour);

            // TODO: this section to be implemented
            // set row/col
            // void setRow(int row, Colour colour);
            // void setRow(int row, const std::vector<Colour>& row_data);

            // void setColumn(int column, Colour colour);
            // void setColumn(int column, const std::vector<Colour>& col_data);

            // // place a sprite at the given position, spirit[0][0] will be placed at the index
            // // this function will cut off the sprite if its placement goes beyond border of the frame
            // // callers responsibilty to check this
            // bool place2dAt(int row, int column, const std::vector< std::vector<Colour> >& sprite);

            // // place a 1d sprite line either horizonally or vertically
            // bool place1dAt(int row, int column, const std::vector< Colour >& sprite, bool horizontal);

            // These function will shift the frame up, down, left, right. Back pixels of the shift are set to black.
            // Note: These methods are destructive. There is no getting back pixels that have been shifted out of frame.
            void shiftUp(size_t shift_by);
            void shiftDown(size_t shift_by);
            void shiftLeft(size_t shift_by);
            void shiftRight(size_t shift_by);

            // diagonals later

            // rotation and flipping

            // Prints formatted contents of the frame
            void print();

            /**
             * @brief Any non BLACK pixels in the right hand operand will overwrite pixels in the left had operand
             *
             * @param rhs
             * @return Frame&
             */
            Frame& operator+=(const Frame& rhs) {
                auto lhs_r = frameBuffer.begin();
                auto lhs_end = frameBuffer.end();
                auto rhs_r = rhs.frameBuffer.begin();
                auto rhs_end = rhs.frameBuffer.end();

                for(;
                    lhs_r != lhs_end && rhs_r != rhs_end;
                    ++lhs_r, ++rhs_r) {

                    // iterator over the colomns
                    auto lhs_c = lhs_r->begin();
                    auto lhs_c_end = lhs_r->end();
                    auto rhs_c = rhs_r->begin();
                    auto rhs_c_end = rhs_r->end();

                    for(;
                        lhs_c != lhs_c_end && rhs_c != rhs_c_end;
                        ++lhs_c, ++rhs_c){

                        if( *rhs_c != BLACK){
                            *lhs_c = *rhs_c;
                        }
                    } // column loop
                }// row loop
                return *this;
            }

            friend std::ostream& operator<<(std::ostream& os, const Frame& f);
            friend class Display;
        private:
            // basic config, hard coded for our setup
            const int HEIGHT = 32;
            const int WIDTH = 32;

            // internal matrix of pixels
            std::vector<std::vector<int>> frameBuffer;

            // so display can access underlying vector and reshape it as it
            // sees fit

    }; // Frame class

    // operator overloads
    std::ostream& operator<<(std::ostream& os, const Frame& f);

} // LED namespace
