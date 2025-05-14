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
#include "primitives.hpp"
#include "descriptor.hxx"

namespace core
{
   /**********************************************************************************************************************
    * 
    *  Control bounds 
    *  Offset: Right Up corner
    *
    **********************************************************************************************************************/
    struct Sector
    {
        const Descriptor* const descriptor;
        Point2D<float> offset { 0.0f, 0.0f };
        const int index { 0 };
        std::unique_ptr<Rectangle<float>[]> bounds[map::cv::count];
        Sector(const Descriptor*, const Point2D<float>&, const int&);
    };
    
    namespace settings {

        extern const int sectors; 
        extern Sector sector_map[];
    }

    struct Grid 
    {
        Sector* sector;
        const int sectors;
        Rectangle<float> bounds { 0.0f, 0.0f, 0.0f, 0.0f };
        float scale { 1.0f };

        Sector* getSector(const core::map::module::type&, const int&);
        void calculate();
        Grid(Sector*, const int, Rectangle<float>);
    };
    
    extern Grid grid;
    
}
