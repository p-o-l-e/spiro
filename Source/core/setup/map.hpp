#pragma once
#include <span>
#include "../interface_headers.hpp"

namespace core 
{
    namespace settings 
    {
        descriptor_t descriptor_map[]
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

        inline const std::span<descriptor_t> default_map(descriptor_map);
    }  
} // Namespace core
