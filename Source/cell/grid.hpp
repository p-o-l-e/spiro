#include "interface_headers.hpp"

namespace cell
{
   /**********************************************************************************************************************
    * 
    *  Control bounds 
    *  Offset: Right Up corner
    *
    **********************************************************************************************************************/
    struct Sector
    {
        const descriptor_t* const descriptor;
        point2d<float> offset { 0.0f, 0.0f };
        const int index { 0 };
        std::unique_ptr<rectangle<float>[]> constrain_i;
        std::unique_ptr<rectangle<float>[]> constrain_o;
        std::unique_ptr<rectangle<float>[]> constrain_c;

        Sector(const descriptor_t* d, const point2d<float>& fs, const int& _index): descriptor(d), offset(fs), index(_index)
        {
            constrain_i = std::make_unique<rectangle<float>[]>(*descriptor->ic);
            constrain_o = std::make_unique<rectangle<float>[]>(*descriptor->oc);          
            constrain_c = std::make_unique<rectangle<float>[]>(*descriptor->cc);
        }
    };
    
    namespace settings {

        inline const int      sectors = 26; 
        inline Sector sector_map[]
        {

            Sector(&interface::vca::descriptor[0]   , point2d<float> { 228.0f, 292.0f }, 1 ),
            Sector(&interface::vca::descriptor[1]   , point2d<float> { 228.0f, 292.0f }, 2 ),

            Sector(&interface::snh::descriptor[0]   , point2d<float> { 532.0f, 292.0f }, 1 ),
            Sector(&interface::snh::descriptor[1]   , point2d<float> { 532.0f, 292.0f }, 2 ),
            
            Sector(&interface::sum::descriptor      , point2d<float> {  76.0f, 292.0f }, 1 ),
            Sector(&interface::sum::descriptor      , point2d<float> { 152.0f, 292.0f }, 2 ),   
            
            Sector(&interface::pdt::descriptor      , point2d<float> {  76.0f, 351.0f }, 1 ),

            Sector(&interface::vco::descriptor      , point2d<float> {   0.0f,   0.0f }, 1 ),
            Sector(&interface::vco::descriptor      , point2d<float> { 152.0f,   0.0f }, 2 ),
            Sector(&interface::vco::descriptor      , point2d<float> { 304.0f,   0.0f }, 3 ),
            Sector(&interface::vco::descriptor      , point2d<float> { 456.0f,   0.0f }, 4 ),
            
            Sector(&interface::map::descriptor      , point2d<float> { 608.0f,   0.0f }, 1 ),
            Sector(&interface::map::descriptor      , point2d<float> { 684.0f,   0.0f }, 2 ),    


            Sector(&interface::vcd::descriptor      , point2d<float> { 304.0f, 292.0f }, 1 ),
 
            Sector(&interface::vcf::descriptor      , point2d<float> { 380.0f, 292.0f }, 1 ),
            Sector(&interface::vcf::descriptor      , point2d<float> { 456.0f, 292.0f }, 2 ),


 
            Sector(&interface::lfo::descriptor      , point2d<float> { 608.0f, 352.0f }, 1 ),
            Sector(&interface::lfo::descriptor      , point2d<float> { 684.0f, 352.0f }, 2 ),
 
            Sector(&interface::rtr::descriptor      , point2d<float> { 760.0f, 270.0f }, 1 ),
            
            Sector(&interface::mix::descriptor      , point2d<float> { 888.0f, 270.0f }, 1 ),
            


            Sector(&interface::env::descriptor[0]   , point2d<float> {   0.0f, 381.0f }, 1 ),
            Sector(&interface::env::descriptor[1]   , point2d<float> {   0.0f, 381.0f }, 2 ),
            Sector(&interface::env::descriptor[2]   , point2d<float> {   0.0f, 381.0f }, 3 ),
            Sector(&interface::env::descriptor[3]   , point2d<float> {   0.0f, 381.0f }, 4 ),

            Sector(&interface::com::descriptor      , point2d<float> {   0.0f, 292.0f }, 1 ),

            Sector(&interface::cro::descriptor      , point2d<float> { 760.0f,   0.0f }, 1 ),
        };
    }

    struct Grid 
    {
        Sector* sector;
        const int sectors;
        rectangle<float> bounds { 0.0f, 0.0f, 0.0f, 0.0f };
        float scale { 1.0f };

        Sector* get_sector(const module_type& t, const int& index)
        {
            for(int i = 0; i < sectors; ++i)
            {
                if(sector[i].index == index)
                    if(sector[i].descriptor->mtype == t)
                        return &sector[i];
            }
            return nullptr;
        }

        void calculate()
        {
            for(int u = 0; u < sectors; ++u)
            {
                for(int i = 0; i < *sector[u].descriptor->ic; ++i)
                {
                    sector[u].constrain_i[i].x = bounds.x + sector[u].offset.x + sector[u].descriptor->set_i[i].constrain.x;
                    sector[u].constrain_i[i].y = bounds.y + sector[u].offset.y + sector[u].descriptor->set_i[i].constrain.y;
                    sector[u].constrain_i[i].w = sector[u].descriptor->set_i[i].constrain.w;
                    sector[u].constrain_i[i].h = sector[u].descriptor->set_i[i].constrain.h;
                }
                for(int i = 0; i < *sector[u].descriptor->oc; ++i)
                {
                    sector[u].constrain_o[i].x = bounds.x + sector[u].offset.x + sector[u].descriptor->set_o[i].constrain.x;
                    sector[u].constrain_o[i].y = bounds.y + sector[u].offset.y + sector[u].descriptor->set_o[i].constrain.y;
                    sector[u].constrain_o[i].w = sector[u].descriptor->set_o[i].constrain.w;
                    sector[u].constrain_o[i].h = sector[u].descriptor->set_o[i].constrain.h;
                }
                for(int i = 0; i < *sector[u].descriptor->cc; ++i)
                {
                    sector[u].constrain_c[i].x = bounds.x + sector[u].offset.x + sector[u].descriptor->set_c[i].constrain.x;
                    sector[u].constrain_c[i].y = bounds.y + sector[u].offset.y + sector[u].descriptor->set_c[i].constrain.y;
                    sector[u].constrain_c[i].w = sector[u].descriptor->set_c[i].constrain.w;
                    sector[u].constrain_c[i].h = sector[u].descriptor->set_c[i].constrain.h;
                }
            }
        }
        
        Grid(Sector* _sector, const int _sectors, rectangle<float> _bounds): sector(_sector), sectors(_sectors), bounds(_bounds) { calculate(); };
    };
    
    inline Grid grid(settings::sector_map, settings::sectors, rectangle<float> { 26.0f, 0.0f, 1060.0f, 596.0f });
    
}
