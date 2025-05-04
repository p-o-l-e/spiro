#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "../../utility/containers.hpp"

namespace core 
{
    namespace map
    {
        struct parameter
        { 
            enum type
            {
                off = 0x00,
                cc  = 0xCC, 
                ic  = 0xAA, 
                oc  = 0xEE 
            };
            static const int count { 4 };
        };

        struct module
        { 
            enum type
            {   
                fuse = 0x00, 
                vco  = 0xFF, 
                lfo  = 0xEE, 
                cso  = 0xDD, 
                vca  = 0xCC, 
                vcd  = 0xBB, 
                vcf  = 0xAA, 
                snh  = 0x99, 
                sum  = 0x88, 
                pdt  = 0x77, 
                rtr  = 0x66, 
                mix  = 0x55, 
                env  = 0x44, 
                mod  = 0x33, 
                com  = 0x22, 
                cro  = 0x11,
            };
            static const int count { 16 };
        };
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
        const map::module::type type = map::module::type::fuse;
        
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
