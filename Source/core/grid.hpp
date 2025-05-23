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

#include <cstdint>
#include <memory>
#ifdef DEBUG_MODE
    #include <iostream>
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
    #define LOG(x)
#endif

#include <array>
#include "primitives.hpp"
#include "descriptor.hxx"
#include "interface_headers.hpp"
#include "uid.hpp"

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
        int index { 0 };
        std::unique_ptr<Rectangle<float>[]> bounds[map::cv::count];
        Sector(const Descriptor*, const Point2D<float>&);
       ~Sector() = default;
    };
    
   /**********************************************************************************************************************
    * 
    *  Grid
    *
    **********************************************************************************************************************/
    class Grid 
    {
        private:
            float scale { 1.0f };
            const std::unique_ptr<int[]> relative;
            const std::unique_ptr<int[]> elements; 
            std::unique_ptr<uint32_t[]> pot_index;
            std::unique_ptr<uint32_t[]> button_index;
            
            const std::unique_ptr<int[]> setRelatives(const Sector*) const;
            const std::unique_ptr<int[]> countElements(const Sector*) const;
            void  calculateUIDs();
            
        public:
            Rectangle<float> bounds { 0.0f, 0.0f, 0.0f, 0.0f };
            const Sector* const sector;
            const int sectors;
            const int pots;
            const int buttons;
            const Sector* getSector(const core::map::module::type&, const int&) const;
            const Rectangle<float>* getBounds(const uid_t&) const;
            const Control* control(const uid_t&) const;
            const int getIndex(const uint32_t&) const;
            const int getIndex(const uid_t&) const;
            const uid_t getUID(const int&, const Control::type&) const;
            Grid(const Sector*, const int&, const Rectangle<float>&);
           ~Grid() = default;
    };
    
    extern Grid grid;

    namespace settings 
    {
        extern Sector sector_map[];
        extern const int sectors;
    }

}

