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
#include "grid.hpp"
#include "uid.hpp"
#include <iostream>

namespace core {
namespace interface { 

    bus_connector::bus_connector(const Grid* grid): blueprint(grid), ccv { grid->count(Control::input), 0, 0 }
    {
        // LOG("BusConnector() : ");
        // for(int i = 0; i < map::cv::count; ++i)
        // {
        //     cv[i] = std::make_unique<std::atomic<float>*[]>(0);
        //     for(int j = 0; j < ccv[i]; ++j) cv[i][j] = &zero;
        // }
        // #ifdef DEBUG 
        //     std::cout<<"---- CC: "<<ccv[map::cv::c]<<" IC: "<<ccv[map::cv::i]<<" OC: "<<ccv[map::cv::o]<<"\n";
        //     std::cout<<"-- Bus initialized...\n";
        // #endif
    }

    std::atomic<float>* bus_connector::pin(const uint32_t& o, const map::cv::index& idx) const 
    { 
        // auto i = blueprint.get_index(o);
        // if(i >= 0) return cv[idx][i]; 
        return &zero;
    }

} // namespace interface
} // namespace core


