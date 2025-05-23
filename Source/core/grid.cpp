
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
#include "grid.hpp"
#include "descriptor.hxx"
#include "uid.hpp"
#include <cstdint>
#include <iostream>
#include <memory>

namespace core
{
    Sector::Sector(const Descriptor* d, const Point2D<float>& fs): descriptor(d), offset(fs)
    {
        for(int j = 0; j < map::cv::count; ++j)
        {
            bounds[j] = std::make_unique<Rectangle<float>[]>(*descriptor->cv[j]);

            for(int i = 0; i < *descriptor->cv[j]; ++i)
            {
                bounds[j][i].x = offset.x + descriptor->set[j][i].constrain.x;
                bounds[j][i].y = offset.y + descriptor->set[j][i].constrain.y;
                bounds[j][i].w = descriptor->set[j][i].constrain.w;
                bounds[j][i].h = descriptor->set[j][i].constrain.h;
            }
        }
    }

    const Sector* Grid::getSector(const core::map::module::type& t, const int& index) const
    {
        for(int i = 0; i < sectors; ++i)
        {
            if(sector[i].index == index)
                if(sector[i].descriptor->type == t)
                    return &sector[i];
        }
        return nullptr;
    }

    Grid::Grid(const Sector* s, const int& size, const Rectangle<float>& b): 
    sector(s), sectors(size), bounds(b), relative(setRelatives(s)), elements(countElements(s)),
    pots(elements[Control::encoder] + elements[Control::slider]),
    buttons(elements[Control::momentary] + elements[Control::toggle] + elements[Control::radio])
    {
        std::cout<<"Potentiometers N : "<<pots<<"\n";
        std::cout<<"Buttons        N : "<<buttons<<"\n\n";
        pot_index = std::make_unique<uint32_t[]>(pots);
        button_index = std::make_unique<uint32_t[]>(buttons);
        std::cout<<"momentary      N : "<<elements[Control::momentary]<<"\n";
        std::cout<<"toggle         N : "<<elements[Control::toggle]<<"\n";
        std::cout<<"radio          N : "<<elements[Control::radio]<<"\n";
        std::cout<<"encoder        N : "<<elements[Control::encoder]<<"\n";
        std::cout<<"slider         N : "<<elements[Control::slider]<<"\n\n";

    };

    void Grid::calculateUIDs()
    {
        int pot = 0;
        int button = 0;
        for(int i = 0; i < sectors; ++i)
        {
            auto mp = relative[i];
            for(int pt = 0; pt < map::cv::count; ++pt)
            {
                for(int pp = 0; pp < *sector[i].descriptor->cv[pt]; ++pp)
                {
                    switch(sector[i].descriptor->set[pt][pp].is) 
                    {
                        case Control::type::encoder:
                        case Control::type::slider:
                            pot_index[pot] = encode_uid(sector[i].descriptor->type, mp, static_cast<map::cv::index>(pt), pp);
                            ++pot;
                            break;
                        case Control::type::momentary:
                        case Control::type::toggle:
                        case Control::type::radio:
                            button_index[button] = encode_uid(sector[i].descriptor->type, mp, static_cast<map::cv::index>(pt), pp);
                            ++button;
                            break;
                            
                        default: break;
                    }
                }
            }
        }
    }

    const int Grid::getIndex(const uid_t& uid) const
    {
        uint32_t hash = encode_uid(uid);

        for(int i = 0; i < buttons; ++i)
        {
            if(button_index[i] == hash) return i;
        }
        for(int i = 0; i < pots; ++i) 
        {
            if(pot_index[i] == hash) return i;
        }
        return -1;
    }

    const uid_t Grid::getUID(const int& index) const
    {
        return decode_uid(pot_index[index]);
    }
    
    const std::unique_ptr<int[]> Grid::setRelatives(const Sector* d) const
    {
        LOG("Grid::set_relatives() :");
        int  n = settings::sectors;
        auto r = std::make_unique<int[]>(n);

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            auto carry = d[s].descriptor->type;
            r[s] = pos;

            for(int i = s + 1; i < n; ++i)
            {
                if(carry == d[i].descriptor->type) 
                {
                    r[i] = ++pos;
                    ++s;
                }
            }
        }
        LOG("---- Relatives set...");
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
        Sector sector_map[]
        {
            Sector(&vca::descriptor[0], Point2D<float>{ 228.0f, 292.0f } ),
            Sector(&vca::descriptor[1], Point2D<float>{ 228.0f, 292.0f } ),
            Sector(&snh::descriptor[0], Point2D<float>{ 532.0f, 292.0f } ),
            Sector(&snh::descriptor[1], Point2D<float>{ 532.0f, 292.0f } ),
            Sector(&sum::descriptor[0], Point2D<float>{  76.0f, 292.0f } ),
            Sector(&sum::descriptor[0], Point2D<float>{ 152.0f, 292.0f } ),   
            Sector(&pdt::descriptor   , Point2D<float>{  76.0f, 351.0f } ),
            Sector(&vco::descriptor   , Point2D<float>{   0.0f,   0.0f } ),
            Sector(&vco::descriptor   , Point2D<float>{ 152.0f,   0.0f } ),
            Sector(&vco::descriptor   , Point2D<float>{ 304.0f,   0.0f } ),
            Sector(&vco::descriptor   , Point2D<float>{ 456.0f,   0.0f } ),
            Sector(&cso::descriptor   , Point2D<float>{ 608.0f,   0.0f } ),
            Sector(&cso::descriptor   , Point2D<float>{ 684.0f,   0.0f } ),    
            Sector(&vcd::descriptor   , Point2D<float>{ 304.0f, 292.0f } ),
            Sector(&vcf::descriptor   , Point2D<float>{ 380.0f, 292.0f } ),
            Sector(&vcf::descriptor   , Point2D<float>{ 456.0f, 292.0f } ),
            Sector(&lfo::descriptor   , Point2D<float>{ 608.0f, 352.0f } ),
            Sector(&lfo::descriptor   , Point2D<float>{ 684.0f, 352.0f } ),
            Sector(&rtr::descriptor   , Point2D<float>{ 760.0f, 270.0f } ),
            Sector(&mix::descriptor   , Point2D<float>{ 888.0f, 270.0f } ),
            Sector(&env::descriptor[0], Point2D<float>{   0.0f, 381.0f } ),
            Sector(&env::descriptor[1], Point2D<float>{   0.0f, 381.0f } ),
            Sector(&env::descriptor[2], Point2D<float>{   0.0f, 381.0f } ),
            Sector(&env::descriptor[3], Point2D<float>{   0.0f, 381.0f } ),
            Sector(&com::descriptor   , Point2D<float>{   0.0f, 292.0f } ),
            Sector(&cro::descriptor   , Point2D<float>{ 760.0f,   0.0f } ),
        };

        const int sectors = std::size(sector_map);
    }


    Grid grid(settings::sector_map, settings::sectors, Rectangle<float> { 26.0f, 0.0f, 1060.0f, 596.0f });   
}


