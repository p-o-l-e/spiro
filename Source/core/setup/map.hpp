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
            interface::map::descriptor    ,
            interface::map::descriptor    ,
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

        inline const std::span<descriptor_t> dm (descriptor_map);

        inline constexpr int n_modules()
        {
            return sizeof(descriptor_map)/sizeof(descriptor_t);
        }
    }  
} // Namespace core
