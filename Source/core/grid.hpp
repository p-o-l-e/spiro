
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
#include "modules/interface/vco_interface.hpp"
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
        const Options* const options;
        const Point2D<float> offset;
        constexpr Sector(const Descriptor* d, const Options* o, const Point2D<float>& f): 
            descriptor(d), options(o), offset(f) {};
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
            const std::unique_ptr<int[]> modules;
            // const int cvs[map::cv::count];
            std::unique_ptr<uint32_t[]>  indices[Control::count];
            const std::unique_ptr<int[]> setRelatives(const Sector*) const;
            const std::unique_ptr<int[]> countElements(const Sector*) const;
            const std::unique_ptr<int[]> countModules(const Sector*) const;
            void  calculateUIDs();
            
        public:
            const Sector* const sector;
            const int sectors;
            constexpr int count(const Control::type& t) const { return elements[t]; }
            constexpr int count(const map::module::type& t) const { return modules[t]; }
            const Rectangle<float> getBounds(const uid_t&) const;
            const std::string name(const uid_t&, const bool) const;
            const Control* control(const uid_t&) const;
            const int getIndex(const uint32_t) const;
            const int getIndex(const uid_t&) const;
            const Sector* getSector(const map::module::type&, const int) const;
            const uid_t getUID(const int, const Control::type&) const;
            const uint32_t getHash(const int, const Control::type&) const;
            Grid(const Sector*, const int);
           ~Grid() = default;
    };
    
    extern const Grid grid;

    namespace settings 
    {
        constexpr Sector sector_map[]
        {
            Sector(&vca::descriptor[0], nullptr      , Point2D<float>{ 228.0f, 292.0f }),
            Sector(&vca::descriptor[1], nullptr      , Point2D<float>{ 228.0f, 292.0f }),
            Sector(&snh::descriptor[0], nullptr      , Point2D<float>{ 532.0f, 292.0f }),
            Sector(&snh::descriptor[1], nullptr      , Point2D<float>{ 532.0f, 292.0f }),
            Sector(&sum::descriptor[0], nullptr      , Point2D<float>{  76.0f, 292.0f }),
            Sector(&sum::descriptor[0], nullptr      , Point2D<float>{ 152.0f, 292.0f }),   
            Sector(&pdt::descriptor   , nullptr      , Point2D<float>{  76.0f, 351.0f }),
            Sector(&vco::descriptor   , &vco::options, Point2D<float>{   0.0f,   0.0f }),
            Sector(&vco::descriptor   , &vco::options, Point2D<float>{ 152.0f,   0.0f }),
            Sector(&vco::descriptor   , &vco::options, Point2D<float>{ 304.0f,   0.0f }),
            Sector(&vco::descriptor   , &vco::options, Point2D<float>{ 456.0f,   0.0f }),
            Sector(&cso::descriptor[0], &cso::options, Point2D<float>{ 608.0f,   0.0f }),
            Sector(&cso::descriptor[1], &cso::options, Point2D<float>{ 684.0f,   0.0f }),    
            Sector(&vcd::descriptor   , nullptr      , Point2D<float>{ 304.0f, 292.0f }),
            Sector(&vcf::descriptor   , nullptr      , Point2D<float>{ 380.0f, 292.0f }),
            Sector(&vcf::descriptor   , nullptr      , Point2D<float>{ 456.0f, 292.0f }),
            Sector(&lfo::descriptor   , &lfo::options, Point2D<float>{ 608.0f, 352.0f }),
            Sector(&lfo::descriptor   , &lfo::options, Point2D<float>{ 684.0f, 352.0f }),
            Sector(&rtr::descriptor   , nullptr      , Point2D<float>{ 760.0f, 270.0f }),
            Sector(&mix::descriptor   , nullptr      , Point2D<float>{ 888.0f, 270.0f }),
            Sector(&env::descriptor[0], nullptr      , Point2D<float>{   0.0f, 381.0f }),
            Sector(&env::descriptor[1], nullptr      , Point2D<float>{   0.0f, 381.0f }),
            Sector(&env::descriptor[2], nullptr      , Point2D<float>{   0.0f, 381.0f }),
            Sector(&env::descriptor[3], nullptr      , Point2D<float>{   0.0f, 381.0f }),
            Sector(&com::descriptor   , nullptr      , Point2D<float>{   0.0f, 292.0f }),
            Sector(&cro::descriptor   , nullptr      , Point2D<float>{ 760.0f,   0.0f }),
        };

        constexpr int sectors = std::size(sector_map);
    }

}

