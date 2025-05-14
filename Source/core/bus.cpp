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
#include "bus.hpp"
#include "uid.hpp"
#include <iostream>

namespace core {
namespace interface { 

    bus_connector::bus_connector(const Descriptor* d): blueprint(d)
    {
        #ifdef DEBUG 
            std::cout<<"-- Initializing bus...\n";
        #endif
        _c = std::make_unique<std::atomic<float>*[]>(blueprint.cc);
        _i = std::make_unique<std::atomic<float>*[]>(blueprint.ic);
        _o = std::make_unique<std::atomic<float>*[]>(blueprint.oc);

        for(int i = 0; i < blueprint.cc; ++i) _c[i] = &zero;
        for(int i = 0; i < blueprint.ic; ++i) _i[i] = &zero;
        for(int i = 0; i < blueprint.oc; ++i) _o[i] = &zero;
        
        #ifdef DEBUG 
            std::cout<<"---- CC: "<<blueprint.cc<<" IC: "<<blueprint.ic<<" OC: "<<blueprint.oc<<"\n";
            std::cout<<"-- Bus initialized...\n";
        #endif
    }

    std::atomic<float>* bus_connector::pin_c(const uint32_t& o) const 
    { 
        auto i = blueprint.get_index(o);
        if(i >= 0) return _c[i]; 
        return &zero;
    }

    std::atomic<float>* bus_connector::pin_i(const uint32_t& o) const 
    { 
        auto i = blueprint.get_index(o);
        if(i >= 0) return _i[i]; 
        return &zero;
    }
    
    std::atomic<float>* bus_connector::pin_o(const uint32_t& o) const 
    { 
        auto i = blueprint.get_index(o);
        if(i >= 0) return _o[i]; 
        return &zero;
    }

} // namespace interface
} // namespace core


