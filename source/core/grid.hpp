
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
#include <sys/types.h>
#include <map>
#include <utility>
#include "interface_headers.hpp"
#include "modules/interface/cro_interface.hpp"
#include "modules/interface/descriptor.hxx"
#include "uid.hpp"
#include "control.hpp"
#include "../assets/svg/svg_modules.hpp"

    
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
            descriptor(d), options(o), offset(f) {
        }
       ~Sector() = default;
    };
    
   /**********************************************************************************************************************
    * 
    *  Grid
    *
    **********************************************************************************************************************/
    class Grid 
    {
        public:
            const size_t sectors;
            const Sector* const sector;

        private:
            const std::unique_ptr<uint8_t[]> relative;
            const std::unique_ptr<size_t[]> elements; 
            const std::unique_ptr<size_t[]> modules;
            std::unique_ptr<uint32_t[]>  indices[Control::count];
            const std::unique_ptr<std::map<uint32_t, const Control*>> controlMap;
            const std::unique_ptr<std::map<uint32_t, std::pair<std::string, std::string>>> idMap;
            const std::unique_ptr<std::map<uint32_t, const Control*>> calculateControlMap(const Sector*) const;
            const std::unique_ptr<std::map<uint32_t, std::pair<std::string, std::string>>> calculateIdMap(const Sector*) const;

            const std::unique_ptr<uint8_t[]> setRelatives(const Sector*) const;
            const std::unique_ptr<size_t[]> countElements(const Sector*) const;
            const std::unique_ptr<size_t[]> countModules(const Sector*) const;
            void  calculateUIDs();
            
        public:
            size_t count(const Control::type& t) const { return elements[t]; }
            int count(const Control::type&, map::flag::type) const;
            size_t count(const map::module::type& t) const { return modules[t]; }
            const Rectangle<float> getBounds(const uid_t&) const;
            const std::string name(const uid_t&, const bool) const;
            const Control* control(const uid_t&) const;
            int getIndex(const uint32_t) const;
            int getIndex(const uid_t&) const;
            const Sector* getSector(const map::module::type&, const int) const;
            const uid_t getUID(size_t, const Control::type&) const;
            uint32_t getHash(size_t, const Control::type&) const;
            Grid(const Sector*, size_t);
            Grid(const std::pair<const Sector*, std::size_t>&);
           ~Grid() = default;
    };
    
    namespace settings 
    {
        inline std::pair<const Sector*, std::size_t> create_sector_map()
        {
            static const Sector sector_map[] 
            {
                Sector(&vca::descriptor[0], nullptr,        { 228.0f, 292.0f}),
                Sector(&vca::descriptor[1], nullptr,        { 228.0f, 292.0f}),
                Sector(&snh::descriptor[0], nullptr,        { 532.0f, 292.0f}),
                Sector(&snh::descriptor[1], nullptr,        { 532.0f, 292.0f}),
                Sector(&sum::descriptor[0], nullptr,        { 76.0f,  292.0f}),
                Sector(&sum::descriptor[0], nullptr,        { 152.0f, 292.0f}),
                Sector(&pdt::descriptor,    nullptr,        {  76.0f, 351.0f}),
                //Sector(new Descriptor(svg::parse_descriptor(svg::assets::vco)), &vco::options, {0.0f, 0.0f}),
                Sector(&vco::descriptor,    &vco::options,  {   0.0f,   0.0f}),
                Sector(&vco::descriptor,    &vco::options,  { 152.0f,   0.0f}),
                Sector(&vco::descriptor,    &vco::options,  { 304.0f,   0.0f}),
                Sector(&vco::descriptor,    &vco::options,  { 456.0f,   0.0f}),
                Sector(&cso::descriptor[0], &cso::options,  { 608.0f,   0.0f}),
                Sector(&cso::descriptor[1], &cso::options,  { 684.0f,   0.0f}),
                Sector(&vcd::descriptor,    nullptr,        { 304.0f, 292.0f}),
                Sector(&vcf::descriptor,    nullptr,        { 380.0f, 292.0f}),
                Sector(&vcf::descriptor,    nullptr,        { 456.0f, 292.0f}),
                Sector(&lfo::descriptor,    &lfo::options,  { 608.0f, 352.0f}),
                Sector(&lfo::descriptor,    &lfo::options,  { 684.0f, 352.0f}),
                Sector(&rtr::descriptor,    nullptr,        { 760.0f, 270.0f}),
                Sector(&mix::descriptor,    nullptr,        { 888.0f, 270.0f}),
                Sector(&env::descriptor[0], nullptr,        {   0.0f, 381.0f}),
                Sector(&env::descriptor[1], nullptr,        {   0.0f, 381.0f}),
                Sector(&env::descriptor[2], nullptr,        {   0.0f, 381.0f}),
                Sector(&env::descriptor[3], nullptr,        {   0.0f, 381.0f}),
                Sector(&com::descriptor,    nullptr,        {   0.0f, 292.0f}),
                Sector(&cro::descriptor,    &cro::options,  { 760.0f,   0.0f}),
            };
            return { sector_map, std::size(sector_map) };
        }
    }

}

