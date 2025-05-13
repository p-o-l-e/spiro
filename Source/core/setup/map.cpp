#include "map.hpp"
#include <cstddef>

namespace core 
{
    namespace settings 
    {
        Descriptor descriptor_map[]
        {
            interface::vco::descriptor    ,
            interface::vco::descriptor    ,
            interface::vco::descriptor    ,
            interface::vco::descriptor    ,

            interface::env::descriptor[0] ,
            interface::env::descriptor[0] ,
            interface::env::descriptor[0] ,
            interface::env::descriptor[0] ,

            interface::lfo::descriptor    ,
            interface::lfo::descriptor    ,
            interface::cso::descriptor    ,
            interface::cso::descriptor    ,

            interface::mix::descriptor    ,
            interface::pdt::descriptor    ,
            interface::sum::descriptor    ,
            interface::rtr::descriptor    ,

            interface::snh::descriptor[0] ,
            interface::snh::descriptor[0] ,
            interface::vca::descriptor[0] ,
            interface::vca::descriptor[0] ,

            interface::vcd::descriptor    ,
            interface::vcf::descriptor    ,
            interface::vcf::descriptor    ,
            interface::cro::descriptor    
        };

        const int modules_n = std::size(descriptor_map);
    }  
} // Namespace core
