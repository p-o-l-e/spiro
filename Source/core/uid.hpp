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
#include <cstdint>
#include "descriptor.hxx"

namespace core
{
    constexpr uint8_t extract_byte(const uint32_t&, const int&) noexcept;
    struct uid_t 
    {
            enum shift { p_position = 0, p_index, m_position, m_index };

            static const uint32_t encode_uid(const map::module::type&, const map::cv::index&, const int&, const int&) noexcept;
            static const uid_t decode_uid(const uint32_t&) noexcept;
            const map::module::type module { map::module::type::off };                      
            const map::cv::index parameter { map::cv::index::off };
            const int module_position { 0 }; 
            const int parameter_position { 0 };
            const uint32_t hash { 0 };

            uid_t() {};
            uid_t(const uint32_t&);
            uid_t(const map::module::type&, const map::cv::index&, const int&, const int&);
           ~uid_t() {};
    };


    /**************************************************************************************************************************
     * Returns 8-bit value
     * Data : 0x FF FF FF FF
     * index:     3  2  1  0     MSB->LSB
     * ***********************************************************************************************************************/
    constexpr uint8_t extract_byte(const uint32_t& data, const int& index) noexcept
    {
        return uint8_t((data >> (8 * (index % 4))) & 0xFF);
    }
}; 
