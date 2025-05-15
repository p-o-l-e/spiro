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
#include "interface_headers.hpp"

namespace core
{
    Sector::Sector(const Descriptor* d, const Point2D<float>& fs, const int& _index): descriptor(d), offset(fs), index(_index)
    {
        for(int i = 0; i < map::cv::count; ++i)
        {
            bounds[i] = std::make_unique<Rectangle<float>[]>(*descriptor->cv[i]);
        }
    }
    
    namespace settings {

        Sector sector_map[]
        {
            Sector(&interface::vca::descriptor[0]   , Point2D<float> { 228.0f, 292.0f }, 1 ),
            Sector(&interface::vca::descriptor[1]   , Point2D<float> { 228.0f, 292.0f }, 2 ),
            Sector(&interface::snh::descriptor[0]   , Point2D<float> { 532.0f, 292.0f }, 1 ),
            Sector(&interface::snh::descriptor[1]   , Point2D<float> { 532.0f, 292.0f }, 2 ),
            Sector(&interface::sum::descriptor[0]   , Point2D<float> {  76.0f, 292.0f }, 1 ),
            Sector(&interface::sum::descriptor[0]   , Point2D<float> { 152.0f, 292.0f }, 2 ),   
            Sector(&interface::pdt::descriptor      , Point2D<float> {  76.0f, 351.0f }, 1 ),
            Sector(&interface::vco::descriptor      , Point2D<float> {   0.0f,   0.0f }, 1 ),
            Sector(&interface::vco::descriptor      , Point2D<float> { 152.0f,   0.0f }, 2 ),
            Sector(&interface::vco::descriptor      , Point2D<float> { 304.0f,   0.0f }, 3 ),
            Sector(&interface::vco::descriptor      , Point2D<float> { 456.0f,   0.0f }, 4 ),
            Sector(&interface::cso::descriptor      , Point2D<float> { 608.0f,   0.0f }, 1 ),
            Sector(&interface::cso::descriptor      , Point2D<float> { 684.0f,   0.0f }, 2 ),    
            Sector(&interface::vcd::descriptor      , Point2D<float> { 304.0f, 292.0f }, 1 ),
            Sector(&interface::vcf::descriptor      , Point2D<float> { 380.0f, 292.0f }, 1 ),
            Sector(&interface::vcf::descriptor      , Point2D<float> { 456.0f, 292.0f }, 2 ),
            Sector(&interface::lfo::descriptor      , Point2D<float> { 608.0f, 352.0f }, 1 ),
            Sector(&interface::lfo::descriptor      , Point2D<float> { 684.0f, 352.0f }, 2 ),
            Sector(&interface::rtr::descriptor      , Point2D<float> { 760.0f, 270.0f }, 1 ),
            Sector(&interface::mix::descriptor      , Point2D<float> { 888.0f, 270.0f }, 1 ),
            Sector(&interface::env::descriptor[0]   , Point2D<float> {   0.0f, 381.0f }, 1 ),
            Sector(&interface::env::descriptor[1]   , Point2D<float> {   0.0f, 381.0f }, 2 ),
            Sector(&interface::env::descriptor[2]   , Point2D<float> {   0.0f, 381.0f }, 3 ),
            Sector(&interface::env::descriptor[3]   , Point2D<float> {   0.0f, 381.0f }, 4 ),
            Sector(&interface::com::descriptor      , Point2D<float> {   0.0f, 292.0f }, 1 ),
            Sector(&interface::cro::descriptor      , Point2D<float> { 760.0f,   0.0f }, 1 ),
        };

        const int sectors = std::size(sector_map);
        const Descriptor* descriptor_map[sectors]; 
    }


    Sector* Grid::getSector(const core::map::module::type& t, const int& index)
    {
        for(int i = 0; i < sectors; ++i)
        {
            if(sector[i].index == index)
                if(sector[i].descriptor->type == t)
                    return &sector[i];
        }
        return nullptr;
    }

    void Grid::calculate()
    {
        for(int u = 0; u < sectors; ++u)
        {
            for(int j = 0; j < map::cv::count; ++j)
            {
                for(int i = 0; i < *sector[u].descriptor->cv[j]; ++i)
                {
                    sector[u].bounds[j][i].x = bounds.x + sector[u].offset.x + sector[u].descriptor->set[j][i].constrain.x;
                    sector[u].bounds[j][i].y = bounds.y + sector[u].offset.y + sector[u].descriptor->set[j][i].constrain.y;
                    sector[u].bounds[j][i].w = sector[u].descriptor->set[j][i].constrain.w;
                    sector[u].bounds[j][i].h = sector[u].descriptor->set[j][i].constrain.h;
                }
            }
        }
    }
        
    Grid::Grid(Sector* _sector, const Descriptor** dmap, const int& _sectors, const Rectangle<float>& _bounds): sector(_sector), sectors(_sectors), bounds(_bounds) 
    { 
        calculate(); 
        for (size_t i = 0; i < sectors; ++i) 
        {
            dmap[i] = sector[i].descriptor;
        }
    };
    
    Grid grid(settings::sector_map, settings::descriptor_map, settings::sectors, Rectangle<float> { 26.0f, 0.0f, 1060.0f, 596.0f });    
}

