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

#ifdef DEBUG_MODE
    #include <iostream>
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
    #define LOG(x)
#endif

#include <cstdint>
#include "descriptor.hxx"

namespace core
{
    enum shift { pp = 0, pt = 8, mp = 16, mt = 24 };

    struct uid_t 
    {
        uint8_t mt: 8;
        uint8_t mp: 8;
        uint8_t pt: 8;
        uint8_t pp: 8;
    };

    /**************************************************************************************************************************
     * Returns 32-bit hash [ AA BB CC DD  ]
     * AA : Module type
     * BB : Module id/position
     * CC : Parameter type
     * DD : Parameter id/position
     * ***********************************************************************************************************************/
    const uint32_t encode_uid(const map::module::type&, const int&, const map::cv::index&, const int&) noexcept;
    const uid_t    decode_uid(const uint32_t&) noexcept;
    /**************************************************************************************************************************
     * Returns 8-bit value
     * Data : 0x FF FF FF FF
     * index:     3  2  1  0     MSB->LSB
     * ***********************************************************************************************************************/
    constexpr uint8_t extract_byte(const uint32_t& data, const shift& index) noexcept
    {
        return uint8_t((data >> index) & 0xFF);
    }
}; 
