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
    void Module::fuse() 
    {
        for(int i = 0; i < _is; ++i) icv[i] = &zero;
        for(int i = 0; i < _cs; ++i) ccv[i] = &zero;
        for(int i = 0; i < _os; ++i) ocv[i].store(0.0f);
    };

    void Module::init(const size_t& ctrls, const size_t& ins, const size_t& outs, const map::module::type& t, const uint8_t& id) noexcept 
    {
        ccv = std::make_unique<std::atomic<float>*[]>(ctrls);
        icv = std::make_unique<std::atomic<float>*[]>(ins);
        ocv = std::make_unique<std::atomic<float> []>(outs);
        _cs = ctrls;
        _is = ins;
        _os = outs;
        fuse();
        type = t;
        position = id;
    }
}
