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
#include "uid.hpp"

namespace core
{
    /**************************************************************************************************************************
     * Returns 32-bit hash [ AA BB CC DD  ]
     * AA : Module type
     * BB : Module id/position
     * CC : Parameter type
     * DD : Parameter id/position
     * ***********************************************************************************************************************/

     const uint32_t uid_t::encode_uid(const map::module::type& mt, const map::cv::index& pt, const int& mp, const int& pp) noexcept
     {
         uint32_t hash = static_cast<uint8_t>(mt);
         hash <<= 8;
         hash += mp;
         hash <<= 8;
         hash += static_cast<uint8_t>(pt);
         hash <<= 8;
         hash += pp;
 
         return hash;
     }

    const uid_t uid_t::decode_uid(const uint32_t& data) noexcept
    {
        return uid_t
        (
            static_cast<map::module::type>(extract_byte(data, m_index)),
            static_cast<map::cv::index>(extract_byte(data, p_index)),
            extract_byte(data, m_position),
            extract_byte(data, p_position)
        );
    }

 
    uid_t::uid_t(const uint32_t& data):
         hash(data),
         module(static_cast<map::module::type>(extract_byte(data, m_index))),
         module_position(extract_byte(data, m_position)),
         parameter(static_cast<map::cv::index>(extract_byte(data, p_index))),
         parameter_position(extract_byte(data, p_position))
     {};
  
    uid_t::uid_t(const map::module::type& mt, const map::cv::index& pt, const int& mp, const int& pp):
         module(mt), parameter(pt), module_position(mp), parameter_position(pp),
         hash(encode_uid(mt, pt, mp, pp))
     {};

}
