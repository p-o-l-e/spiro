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

#include "constants.hpp"
#include "grid.hpp"
#include "interface_headers.hpp"

#ifdef DEBUG_MODE
    #include <iostream>
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
    #define LOG(x)
#endif


namespace core {
    enum error { invalid_index = -1 };
namespace interface {

	class Blueprint
    {
        private:
            const int count(const map::cv::index&, const Descriptor**) const;
            const std::unique_ptr<int[]> set_relatives(const Descriptor**) const;
            void calculate_hash();
            std::unique_ptr<uint32_t[]> hash_table[map::cv::count]; 


        public:
            const Control get_control(const uint32_t&) const;
            const int get_index(const uint32_t&) const;
            const uint32_t get_hash(const map::cv::index&, const int&) const;
            const Descriptor** descriptor;
            const std::unique_ptr<int[]> relative; 
            const int ccv[map::cv::count];
            const int mc;

            Blueprint(const Descriptor**);
           ~Blueprint() { LOG("~Blueprint()"); };
    };
}
} // Namespace core
