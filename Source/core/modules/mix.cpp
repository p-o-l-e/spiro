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
#include "mix.hpp"

namespace core
{
    using namespace interface::mix;
    int mixer::idc = 0;
    
            void mixer::process()
            {
                point3d<float> lcr
                { 
                    in[cvi::l]->load(), 
                    in[cvi::c]->load(), 
                    in[cvi::r]->load() 
                };
                float lc = ctrl[ctl::alpha]->load() + in[cvi::alpha]->load();
                float cr = ctrl[ctl::theta]->load() + in[cvi::theta]->load();
                point2d<float> lr = c3c2(lcr, lc, cr);
                out[cvo::l].store(lr.x * ctrl[ctl::amp]->load());
                out[cvo::r].store(lr.y * ctrl[ctl::amp]->load());
            }

            mixer::mixer(): id(++idc)
            {
                init(cc, ic, oc, module_type::mixer, id);
            }
            mixer::~mixer() {};


}; // Namespace
