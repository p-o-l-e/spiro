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
#include "constants.hpp"
#include "containers.hpp"
#include "utility.hpp"

namespace core
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace mxr       
        {
            const int ctrls  { 3 };
            const int ins    { 5 };
            const int outs   { 2 };

            enum class ctrl { alpha, theta, volume };
            enum class in   { l, c, r, alpha, theta };
            enum class out  { l, r };

        }
    }

    class mixer: public module
    {
        private:
            static int idc;

        public:
            int id;
            void process() override
            {
                point3d<float> a 
                { 
                    in[static_cast<int>(interface::mxr::in::l)]->load(), 
                    in[static_cast<int>(interface::mxr::in::c)]->load(), 
                    in[static_cast<int>(interface::mxr::in::r)]->load() 
                };
                float lc = ctrl[static_cast<int>(interface::mxr::ctrl::alpha)]->load() + in[static_cast<int>(interface::mxr::in::alpha)]->load();
                float cr = ctrl[static_cast<int>(interface::mxr::ctrl::theta)]->load() + in[static_cast<int>(interface::mxr::in::theta)]->load();
                point2d<float> lr = c3c2(a, lc, cr);
                out[static_cast<int>(interface::mxr::out::l)].store(lr.x * ctrl[static_cast<int>(interface::mxr::ctrl::volume)]->load());
                out[static_cast<int>(interface::mxr::out::r)].store(lr.y * ctrl[static_cast<int>(interface::mxr::ctrl::volume)]->load());
            }

            mixer(): id(++idc)
            {
                init(interface::mxr::ctrls, interface::mxr::ins, interface::mxr::outs, module_type::mixer, id);
            }
           ~mixer() {};
    };

}; // Namespace
