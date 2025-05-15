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
#include "node.hpp"
#include <cstdint>

namespace core
{

    void module::set_uid(const module_type& t, const uint8_t& id)
    {
        int i { 0 };
        int n { 0 };
        uint32_t mi = get_module_index(module_type::vco, id);
        mi <<= 16;
        for(; i < _cs; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::ctrl, n) };
            uid[n] = f;
            ++n;
        }
        for(; i < _is; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::in, n) };
            uid[n] = f;
            ++n;
        }
        for(; i < _os; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::out, n) };
            uid[n] = f;
            ++n;
        }
    }

    module::~module() = default;

    std::atomic<float>* rack_t::get_output_pin(const uint32_t&) const
    {

    }

    std::atomic<float>* rack_t::get_output_pin(const module_type& t, const int& module_pos, const int& pin_pos)
    {
        int i = 0;
        for(; i < _size; ++i)
        {
            if(node[i]->type == t)
            {
                if(node[i]->position == module_pos)
                {
                    break;
                }
            }
        }
        return &node[i]->out[pin_pos];
    }

    std::atomic<float>** rack_t::get_input_pin(const uint32_t&) const
    {
    }

    std::atomic<float>** rack_t::get_input_pin(const module_type& t, const int& module_pos, const int& pin_pos)
    {
        int i = 0;
        for(; i < _size; ++i)
        {
            if(node[i]->type == t)
            {
                if(node[i]->position == module_pos)
                {
                    break;
                }
            }
        }
        return &node[i]->in[pin_pos];
    }

    std::atomic<float>** rack_t::get_ctrl_pin(const uint32_t&) const
    {
    }

    std::atomic<float>** rack_t::get_ctrl_pin(const module_type& t, const int& module_pos, const int& pin_pos)
    {
        int i = 0;
        for(; i < _size; ++i)
        {
            if(node[i]->type == t)
            {
                if(node[i]->position == module_pos)
                {
                    break;
                }
            }
        }
        return &node[i]->ctrl[pin_pos];
    }


    /**************************************************************************************************************************
     * Returns 16-bit index [ MSB LSB ]
     * MSB : Module type
     * LSB : Module id/position
     * ***********************************************************************************************************************/
    constexpr uint16_t get_module_index(const module_type& t, const uint8_t& id)
    {
        uint16_t r = static_cast<uint8_t>(t);

        if(r)
        {
            r <<= 8;
            r += id;
        }
        return r;
    }
    
    /**************************************************************************************************************************
     * Returns 16-bit index [ MSB LSB ]
     * MSB : Parameter type
     * LSB : Parameter id/position
     * ***********************************************************************************************************************/
    constexpr uint16_t get_parameter_index(const parameter_type& t, const uint8_t& id)
    {
        uint16_t r = static_cast<uint8_t>(t);

        if(r)
        {
            r <<= 8;
            r += id;
        }
        return r;
    }

    void  encode_uid(uid_t* uid) noexcept
    {
        uint16_t msb = get_module_index(uid->module, uid->module_position);
        uint16_t lsb = get_parameter_index(uid->parameter, uid->parameter_position);
        uid->key = 0;
        uid->key <<= msb;
        uid->key <<= lsb;
    }

    uid_t decode_uid(const uint64_t) noexcept
    {

    }



}
