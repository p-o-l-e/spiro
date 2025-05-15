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
#include "containers.hpp"
#include "constants.hpp"
#include "node.hpp"

namespace core
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace rtr
        {
            const int ctrls  { 3 };
            const int ins    { 9 };
            const int outs   { 6 };

            enum class ctrl {  x,  y,  z };
            enum class in   { ax, ay, az, bx, by, bz, cvx, cvy, cvz };
            enum class out  { ax, ay, az, bx, by, bz };

        }
    }

using namespace core::interface::rtr;

class rotor: public module
{
    private:
        static int idc;

    public:
        int id;
        quaternion q;

        void process() override
        {
            point3d<float> a 
            { 
                in[static_cast<int>(interface::rtr::in::ax)]->load() + 
                in[static_cast<int>(interface::rtr::in::bx)]->load(),
                in[static_cast<int>(interface::rtr::in::ay)]->load() +
                in[static_cast<int>(interface::rtr::in::by)]->load(),
                in[static_cast<int>(interface::rtr::in::az)]->load() +
                in[static_cast<int>(interface::rtr::in::bz)]->load() 
            };

            float x = ctrl[static_cast<int>(interface::rtr::ctrl::x)]->load() + pi * in[static_cast<int>(interface::rtr::in::cvx)]->load();
            float y = ctrl[static_cast<int>(interface::rtr::ctrl::y)]->load() + pi * in[static_cast<int>(interface::rtr::in::cvy)]->load();
            float z = ctrl[static_cast<int>(interface::rtr::ctrl::z)]->load() + pi * in[static_cast<int>(interface::rtr::in::cvz)]->load();

            q.from_euler(x, y, z);
            q.rotate_vector(a.x, a.y, a.z);

            out[static_cast<int>(interface::rtr::out::ax)].store(a.x);
            out[static_cast<int>(interface::rtr::out::ay)].store(a.y);
            out[static_cast<int>(interface::rtr::out::az)].store(a.z);

            out[static_cast<int>(interface::rtr::out::bx)].store(a.x);
            out[static_cast<int>(interface::rtr::out::by)].store(a.y);
            out[static_cast<int>(interface::rtr::out::bz)].store(a.z);

        }

        rotor(): id(++idc)
        { 
            init(interface::rtr::ctrls, interface::rtr::ins, interface::rtr::outs, module_type::rotor, id);
        };
       ~rotor() {};
};

}
