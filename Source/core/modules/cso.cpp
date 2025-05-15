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

#include "cso.hpp"

namespace core {

int map_t::idc = 0;

void map_t::process()
{
    int f = (int)ccv[static_cast<int>(interface::cso::ctrl::form)]->load();

    if(_reset)
    {
        (this->*reset[0])();
        _reset = false;
    }
    (this->*form[0])();
}

void map_t::switch_wave(const int& w)
{
    _reset = true;
}

map_t::map_t(): id(++idc)
{
    init(interface::cso::ctrls, interface::cso::ins, interface::cso::outs, module_type::cso, id);
}

void map_t::sprott_reset()
{
    f[0] = 0.8f;     // a
    f[1] = 0.5f;     // b
    f[2] = 0.1f;     // c
    f[3] = 1.0f;     // d
    f[4] = 0.01f;    // t

    f[5] = 0.1f;     // x
    f[6] = 0.1f;     // y
    f[7] = 0.1f;     // z
}

void map_t::sprott() 
{
    f[4] = (ccv[static_cast<int>(interface::cso::ctrl::tune)]->load() + ccv[static_cast<int>(interface::cso::in::fm)]->load() * 0.1f ) * 0.2f + 0.0001f;
    if(ccv[static_cast<int>(interface::cso::in::warp)] == &zero) f[2] = 0.1f + ccv[static_cast<int>(interface::cso::ctrl::warp)]->load();
    else f[2] = 0.1f + ccv[static_cast<int>(interface::cso::ctrl::warp)]->load() * ccv[static_cast<int>(interface::cso::in::warp)]->load();

    f[5] += f[4] * f[6] * f[0];
    f[6] += f[4] * (- f[6] * f[7] - f[5]);
    f[7] += f[4] * (f[1] * f[6] * f[6] - f[2] * f[5] - f[3]);

    if((f[5] > std::numeric_limits<float>::max()) && (f[6] > std::numeric_limits<float>::max()) && (f[7] > std::numeric_limits<float>::max())) 
    {
        f[5] = 0.1f; 
        f[6] = 0.1f;
        f[7] = 0.1f;
    }
    else
    {
        ocv[static_cast<int>(interface::cso::out::x)].store(f[5] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.4f);
        ocv[static_cast<int>(interface::cso::out::y)].store(f[6] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.4f);
        ocv[static_cast<int>(interface::cso::out::z)].store(f[7] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.4f);
    }
}

void map_t::helmholz_reset()
{
    f[5] = 0.1f;     // x
    f[6] = 0.1f;     // y
    f[7] = 0.1f;     // z

    f[0] = 5.11f;    // Gamma
    f[1] = 0.55f;    // Delta
    f[2] = 0.01;     // t
}

void map_t::helmholz() 
{
    f[2] = (ccv[static_cast<int>(interface::cso::ctrl::tune)]->load() + ccv[static_cast<int>(interface::cso::in::fm)]->load() * 0.1f ) * 0.2f + 0.01f;
    if(ccv[static_cast<int>(interface::cso::in::warp)] == &zero) f[1] = ((ccv[static_cast<int>(interface::cso::ctrl::warp)]->load() - 0.5f) * 0.03f) + 0.55f;
    else f[1] = ((ccv[static_cast<int>(interface::cso::ctrl::warp)]->load() * fabsf(ccv[static_cast<int>(interface::cso::in::warp)]->load()) - 0.5f) * 0.03f) + 0.55f;

        f[5] += f[2] * f[6];
        f[6] += f[2] * f[0] * f[7];
        f[7] += f[2] * ( -f[7] - f[1] * f[6] - f[5] - f[5] * f[5] );

    if((f[5] > std::numeric_limits<float>::max()) && (f[6] > std::numeric_limits<float>::max()) && (f[7] > std::numeric_limits<float>::max())) 
    {
        f[5] = 0.1f; 
        f[6] = 0.1f;
        f[7] = 0.1f;
    }
    else
    {     
        ocv[static_cast<int>(interface::cso::out::x)].store(f[5] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 3.0f);
        ocv[static_cast<int>(interface::cso::out::y)].store(f[6] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 3.0f);
        ocv[static_cast<int>(interface::cso::out::z)].store(f[7] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 3.0f);
    }
}

void map_t::halvorsen_reset()
{
    f[0] = 1.4f;     // a
    f[1] = 0.01f;    // t

    f[5] = 0.1f;     // x
    f[6] = 0.0f;     // y
    f[7] = 0.0f;     // z
};


void map_t::halvorsen()
{
    f[1] = (ccv[static_cast<int>(interface::cso::ctrl::tune)]->load() + ccv[static_cast<int>(interface::cso::in::fm)]->load() * 0.01f) * 0.02f + 0.00001f;
    if(ccv[static_cast<int>(interface::cso::in::warp)] == &zero) f[0] = 1.4f + ccv[static_cast<int>(interface::cso::ctrl::warp)]->load();
    else f[0] = 1.4f + ccv[static_cast<int>(interface::cso::ctrl::warp)]->load() * fabsf(ccv[static_cast<int>(interface::cso::in::warp)]->load());

    f[5] += f[1] * ( - f[0] * f[5] - 4.0f * f[6] - 4.0f * f[7] - f[6] * f[6]);
    f[6] += f[1] * ( - f[0] * f[6] - 4.0f * f[7] - 4.0f * f[5] - f[7] * f[7]);
    f[7] += f[1] * ( - f[0] * f[7] - 4.0f * f[5] - 4.0f * f[6] - f[5] * f[5]);

    if((f[5] > std::numeric_limits<float>::max()) && (f[6] > std::numeric_limits<float>::max()) && (f[7] > std::numeric_limits<float>::max())) 
    {
        f[5] = 0.1f; 
        f[6] = 0.0f;
        f[7] = 0.0f;
    }
    else
    {
        ocv[static_cast<int>(interface::cso::out::x)].store(f[5] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.5f);
        ocv[static_cast<int>(interface::cso::out::y)].store(f[6] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.5f);
        ocv[static_cast<int>(interface::cso::out::z)].store(f[7] * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.5f);
    }

}

void map_t::tsucs_reset()
{
    f[0] = 1.0f;     // x
    f[1] = 1.0f;     // y
    f[2] = 1.0f;     // z

    f[3] = 40.00f;   // a
    f[4] = 0.500f;   // b
    f[5] = 20.00f;   // c
    f[6] = 0.833f;   // d
    f[7] = 0.650f;   // e

    f[8] = 0.001f;   // t
};

void map_t::tsucs()
{
    if(ccv[static_cast<int>(interface::cso::in::warp)] == &zero) f[7] = ccv[static_cast<int>(interface::cso::ctrl::warp)]->load() / 8.0f + 0.55f;
    else f[7] = ccv[static_cast<int>(interface::cso::ctrl::warp)]->load() * fabsf(ccv[static_cast<int>(interface::cso::in::warp)]->load())  / 8.0f + 0.55f;

    f[8] = (ccv[static_cast<int>(interface::cso::ctrl::tune)]->load() + ccv[static_cast<int>(interface::cso::in::fm)]->load() * 0.01f) / 100.0f + 0.00001f;

        f[0] += f[8] * (f[3] * (f[1] - f[0]) + f[4] * f[0] * f[2]);
        f[1] += f[8] * (f[5] *  f[1] - f[0]  * f[2]);
        f[2] += f[8] * (f[6] *  f[2] + f[0]  * f[1] - f[7] * f[0] * f[0]);
 
    if((f[0] > std::numeric_limits<float>::max()) && (f[1] > std::numeric_limits<float>::max()) && (f[2] > std::numeric_limits<float>::max())) 
    {
        f[0] = 1.0f; 
        f[1] = 1.0f;
        f[2] = 1.0f;
    }
    else
    {
        ocv[static_cast<int>(interface::cso::out::x)].store((f[0]) * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.05f);
        ocv[static_cast<int>(interface::cso::out::y)].store((f[1]) * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.05f);
        ocv[static_cast<int>(interface::cso::out::z)].store((f[2] - 45.0f) * ccv[static_cast<int>(interface::cso::ctrl::amp)]->load() * 0.05f);
    }
}

}
