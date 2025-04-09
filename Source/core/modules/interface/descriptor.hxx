#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "../../utility/containers.hpp"

namespace core 
{
    enum class parameter_type { cc = 0xCC, ic = 0xAA, oc = 0xEE, off = 0x00 };
    enum class module_type { fuse, vco, lfo, map, vca, vcd, vcf, snh, sum, pdt, rtr, mix, env, mod, com, cro };

    namespace settings 
    {
       inline const int  module_types = 16; 
    }

    struct control_t
    {
        enum class type_t                 { fuse, slider, encoder, push, momentary, radio, parameter, socket, pin };
        const control_t::type_t type      { control_t::type_t::fuse };
        const rectangle<float>  constrain { 0.0f, 0.0f, 0.0f, 0.0f };
        const std::string       postfix   { "fuse" };
        
        const float min         { 0.0f  };                      // Min value
        const float max         { 1.0f  };                      // Max value
        const float def         { 0.0f  };                      // Default value

        const float skew        { 0.5f  };
        const float step        { 0.0f  };
 
        const int   radio_id    { 0     };
        const bool  symmetric   { false };

        const uint32_t flag     { 0     };                      // Control specific settings
    };



    struct descriptor_t 
    {
        const module_type mtype = module_type::fuse;
        
        const int* const ic;                                    // Inputs
        const int* const oc;                                    // Outputs
        const int* const cc;                                    // Controls

        const std::string* const prefix;                        // Module name
        
        const control_t* const set_i;
        const control_t* const set_o;
        const control_t* const set_c;
        
        const rectangle<float>* const constrain; 
    };
}
