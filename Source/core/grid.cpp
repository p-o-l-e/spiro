
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
#include <cstddef>
#include <iostream>
#include "grid.hpp"
#include "cro_interface.hpp"
#include "descriptor.hxx"
#include "primitives.hpp"

namespace core
{
    Grid::Grid(const Sector* s, const int& size, const Rectangle<float>& b): 
    sector(s), sectors(size), bounds(b), relative(setRelatives(s)), elements(countElements(s))
    {
        for(int i = 0; i < Control::count; ++i) indices[i] = std::make_unique<uint32_t[]>(elements[i]);
        calculateUIDs();
    };

    void Grid::calculateUIDs()
    {
        int count[Control::count];
        for(int i = 0; i < Control::count; ++i) count[i] = 0;

        for(int i = 0; i < sectors; ++i)
        {
            auto mp = relative[i];
            for(int pt = 0; pt < map::cv::count; ++pt)
            {
                for(int pp = 0; pp < *sector[i].descriptor->cv[pt]; ++pp)
                {
                    auto t = sector[i].descriptor->set[pt][pp].is;
                    indices[t][count[t]] = encode_uid(sector[i].descriptor->type, mp, static_cast<map::cv::index>(pt), pp);
                    ++count[t];
                }
            }
        }
    }

    const int Grid::getIndex(const uid_t& uid) const
    {
        uint32_t hash = encode_uid(uid);
        auto t = control(uid)->is;
        for(int i = 0; i < elements[t]; ++i)
        {
            if(indices[t][i] == hash) return i;
        }
        return -1;
    }

    const Control* Grid::control(const uid_t& uid) const
    {
        for(int i = 0; i < sectors; ++i)
        {
            if(sector[i].descriptor->type == static_cast<map::module::type>(uid.mt))
            {
                if(relative[i] == uid.mp) return &sector[i].descriptor->set[uid.pt][uid.pp];
            }
        }
        return nullptr;
    }

    const uid_t Grid::getUID(const int& index, const Control::type& type) const
    {
        return decode_uid(indices[type][index]);
    }

    const uint32_t Grid::getHash(const int& index, const Control::type& type) const
    {
        return indices[type][index];
    }

    const Rectangle<float> Grid::getBounds(const uid_t& uid) const
    {
        Rectangle<float> bounds { 0, 0, 0, 0 };
        for(int i = 0; i < sectors; ++i)
        {
            if(sector[i].descriptor->type == static_cast<map::module::type>(uid.mt))
            {
                if(relative[i] == uid.mp) 
                {
                    bounds.x = sector[i].offset.x + sector[i].descriptor->set[uid.pt][uid.pp].constrain.x;
                    bounds.y = sector[i].offset.y + sector[i].descriptor->set[uid.pt][uid.pp].constrain.y;
                    bounds.w = sector[i].descriptor->set[uid.pt][uid.pp].constrain.w;
                    bounds.h = sector[i].descriptor->set[uid.pt][uid.pp].constrain.h;
                    return bounds;
                }
            }
        }
        return bounds;
    }


    const std::unique_ptr<int[]> Grid::setRelatives(const Sector* d) const
    {
        int  n = settings::sectors;
        auto r = std::make_unique<int[]>(n);
        bool check[n];
        for(int i = 0; i < n; ++i) check[i] = false;

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            auto carry = d[s].descriptor->type;

            for(int i = s; i < n; ++i)
            {
                if(!check[i])
                {
                    if(carry == d[i].descriptor->type) 
                    {
                        r[i] = pos;
                        check[i] = true;
                        ++pos;
                    }
                }
            }
        }
        return r;
    }
    
    const std::unique_ptr<int[]> Grid::countElements(const Sector* d) const
    {
        int  length = settings::sectors;
        auto r = std::make_unique<int[]>(Control::count);
        for(int i = 0; i < Control::count; ++i) r[i] = 0;
        for(int i = 0; i < length; ++i)
        {
            for(int t = 0; t < map::cv::count; ++t)
            {
                for(int e = 0; e < *d[i].descriptor->cv[t]; ++e)
                {   
                    ++r[d[i].descriptor->set[t][e].is];
                }
            }
        }
        return r;
    }


    namespace settings 
    {
        const Sector sector_map[]
        {
            Sector(&vca::descriptor[0], Point2D<float>{ 228.0f, 292.0f }),
            Sector(&vca::descriptor[1], Point2D<float>{ 228.0f, 292.0f }),
            Sector(&snh::descriptor[0], Point2D<float>{ 532.0f, 292.0f }),
            Sector(&snh::descriptor[1], Point2D<float>{ 532.0f, 292.0f }),
            Sector(&sum::descriptor[0], Point2D<float>{  76.0f, 292.0f }),
            Sector(&sum::descriptor[0], Point2D<float>{ 152.0f, 292.0f }),   
            Sector(&pdt::descriptor   , Point2D<float>{  76.0f, 351.0f }),
            Sector(&vco::descriptor   , Point2D<float>{   0.0f,   0.0f }),
            Sector(&vco::descriptor   , Point2D<float>{ 152.0f,   0.0f }),
            Sector(&vco::descriptor   , Point2D<float>{ 304.0f,   0.0f }),
            Sector(&vco::descriptor   , Point2D<float>{ 456.0f,   0.0f }),
            Sector(&cso::descriptor[0], Point2D<float>{ 608.0f,   0.0f }),
            Sector(&cso::descriptor[1], Point2D<float>{ 684.0f,   0.0f }),    
            Sector(&vcd::descriptor   , Point2D<float>{ 304.0f, 292.0f }),
            Sector(&vcf::descriptor   , Point2D<float>{ 380.0f, 292.0f }),
            Sector(&vcf::descriptor   , Point2D<float>{ 456.0f, 292.0f }),
            Sector(&lfo::descriptor   , Point2D<float>{ 608.0f, 352.0f }),
            Sector(&lfo::descriptor   , Point2D<float>{ 684.0f, 352.0f }),
            Sector(&rtr::descriptor   , Point2D<float>{ 760.0f, 270.0f }),
            Sector(&mix::descriptor   , Point2D<float>{ 888.0f, 270.0f }),
            Sector(&env::descriptor[0], Point2D<float>{   0.0f, 381.0f }),
            Sector(&env::descriptor[1], Point2D<float>{   0.0f, 381.0f }),
            Sector(&env::descriptor[2], Point2D<float>{   0.0f, 381.0f }),
            Sector(&env::descriptor[3], Point2D<float>{   0.0f, 381.0f }),
            Sector(&com::descriptor   , Point2D<float>{   0.0f, 292.0f }),
            Sector(&cro::descriptor   , Point2D<float>{ 760.0f,   0.0f }),
        };

        const int sectors = std::size(sector_map);
    }


    Grid grid(settings::sector_map, settings::sectors, Rectangle<float> { 27.0f, 0.0f, 1060.0f, 596.0f });   
}


