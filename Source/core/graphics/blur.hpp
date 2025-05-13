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

#include "../utility/containers.hpp"

namespace core {

constexpr void hBlur(frame<float>* data, const unsigned& scale = 1)
{
    int range = scale * 2 + 1;
    for(unsigned y = 0; y < data->height; y++)
    {
        for(unsigned x = 0; x < data->width-range; x++)
        {
            float carry = 0.0f;
            for(int i = 0; i < range; i++)
            {   
                carry += data->get(x + i, y);
            }
            carry /= range;
            data->set(x + scale, y, carry);
        }
    }
}

constexpr void vBlur(frame<float>* data, const unsigned& scale = 1)
{
    int range = scale * 2 + 1;  
    for(unsigned x = 0; x < data->width; x++)
    {
        for(unsigned y = 0; y < data->height-range; y++)
        {
            float carry = 0.0f;
            for(int i = 0; i < range; i++)
            {   
                carry += data->get(x, y + i);
            }
            carry /= range;
            data->set(x, y + scale, carry);
        }
    }
}

constexpr void boxBlur(frame<float>* data, const unsigned& scale = 1)
{
    int range = scale * 2 + 1;
    for(unsigned y = 0; y < data->height; y++)
    {
        for(unsigned x = 0; x < data->width-range; x++)
        {
            float carry = 0.0f;
            for(int i = 0; i < range; i++)
            {   
                carry += data->get(x + i, y);
            }
            carry /= float(range);
            data->set(x + scale, y, carry);
        }
    }
    
    for(unsigned x = 0; x < data->width; x++)
    {
        for(unsigned y = 0; y < data->height-range; y++)
        {
            float carry = 0.0f;
            for(int i = 0; i < range; i++)
            {   
                carry += data->get(x, y + i);
            }
            carry /= float(range);
            data->set(x, y + scale, carry);
        }
    }
}

};

