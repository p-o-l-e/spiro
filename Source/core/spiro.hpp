/*****************************************************************************************************************************
* Copyright (c) 2022-2025 POLE
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************************************************************/
#pragma once

#include "node.hpp"
#ifdef DEBUG_MODE
    #include <iostream>
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
    #define LOG(x)
#endif

#include <iostream>
#include <cstdlib>

#include "grid.hpp"
#include "modmatrix.hpp"
#include "utility.hpp"
#include "rack.hpp"
#include "iospecs.hpp"
#include "module_headers.hpp"

namespace core 
{
    using namespace interface;

    class Spiro
    { 
        public:
            struct stereo { enum { l, r }; };
        private:
            bool standby = false;
            Module* mixer; 
            void note_on (uint8_t, uint8_t);
            void note_off(uint8_t);
            void connect_bus();
            void arm();

        public:
            const Grid* grid;
            Rack rack;
            DCBlock dcb[2];
            Patchbay* bay = nullptr;
            std::atomic<float> out[2];                       // LR Output
            void midi_message(uint8_t, uint8_t, uint8_t);
            void process() noexcept;
            Spiro(const Grid*);
           ~Spiro();
    };
};
