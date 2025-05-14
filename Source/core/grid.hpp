#pragma once
#include "interface_headers.hpp"

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

        Sector(const Descriptor* d, const Point2D<float>& fs, const int& _index): descriptor(d), offset(fs), index(_index)
        {
            for(int i = 0; i < map::cv::count; ++i)
            {
                bounds[i] = std::make_unique<Rectangle<float>[]>(*descriptor->cv[i]);
            }
        }
    };
    
    namespace settings {

        inline const int sectors = 26; 
        inline Sector sector_map[]
        {
            Sector(&interface::vca::descriptor[0]   , Point2D<float> { 228.0f, 292.0f }, 1 ),
            Sector(&interface::vca::descriptor[1]   , Point2D<float> { 228.0f, 292.0f }, 2 ),
            Sector(&interface::snh::descriptor[0]   , Point2D<float> { 532.0f, 292.0f }, 1 ),
            Sector(&interface::snh::descriptor[1]   , Point2D<float> { 532.0f, 292.0f }, 2 ),
            Sector(&interface::sum::descriptor      , Point2D<float> {  76.0f, 292.0f }, 1 ),
            Sector(&interface::sum::descriptor      , Point2D<float> { 152.0f, 292.0f }, 2 ),   
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
    }

    struct Grid 
    {
        Sector* sector;
        const int sectors;
        Rectangle<float> bounds { 0.0f, 0.0f, 0.0f, 0.0f };
        float scale { 1.0f };

        Sector* get_sector(const core::map::module::type& t, const int& index)
        {
            for(int i = 0; i < sectors; ++i)
            {
                if(sector[i].index == index)
                    if(sector[i].descriptor->type == t)
                        return &sector[i];
            }
            return nullptr;
        }

        void calculate()
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
        
        Grid(Sector* _sector, const int _sectors, Rectangle<float> _bounds): sector(_sector), sectors(_sectors), bounds(_bounds) { calculate(); };
    };
    
    inline Grid grid(settings::sector_map, settings::sectors, Rectangle<float> { 26.0f, 0.0f, 1060.0f, 596.0f });
    
}
