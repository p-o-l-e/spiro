
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
#include <string>
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
        const Point2D<float> offset;
        constexpr Sector(const Descriptor* d, const Point2D<float>& f): 
            descriptor(d), offset(f) {};
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
            const std::unique_ptr<int[]> relative;
            const std::unique_ptr<int[]> elements; 
            // const int cvs[map::cv::count];
            std::unique_ptr<uint32_t[]>  indices[Control::count];
            const std::unique_ptr<int[]> setRelatives(const Sector*) const;
            const std::unique_ptr<int[]> countElements(const Sector*) const;
            void  calculateUIDs();
            
        public:
            const Sector* const sector;
            const int sectors;
            constexpr int count(const Control::type& t) const { return elements[t]; }
            const Rectangle<float> getBounds(const uid_t&) const;
            const std::string name(const uid_t&, const bool&) const;
            const Control* control(const uid_t&) const;
            const int getIndex(const uint32_t&) const;
            const int getIndex(const uid_t&) const;
            const uid_t getUID(const int&, const Control::type&) const;
            const uint32_t getHash(const int&, const Control::type&) const;
            Grid(const Sector*, const int&);
           ~Grid() = default;
    };
    
    extern const Grid grid;

    namespace settings 
    {
        extern const Sector sector_map[];
        extern const int sectors;
    }

}

