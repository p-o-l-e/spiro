#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "primitives.hpp"
#include "constants.hpp"

namespace core 
{
    namespace map
    {
        struct cv
        {
            enum index { i, o, c, count, off = 0xFF };
        };

        struct module
        { 
            enum type
            {   
                off, 
                vco, 
                lfo, 
                cso, 
                vca, 
                vcd, 
                vcf, 
                snh, 
                sum, 
                pdt, 
                rtr, 
                mix, 
                env, 
                mod, 
                com, 
                cro,
            };
            static const int count { 16 };
        };
        
        struct flag 
        {
            enum type 
            {
                A           = 1 << 0,
                B           = 1 << 1,
                encoder     = 1 << 2,
                toggle      = 1 << 3,
                momentary   = 1 << 4,
                radio       = 1 << 5,
            };
        };
    }

    struct Control
    {
        enum  type                        { slider, button, parameter, input, output, pin, count };
        const Control::type     is        { parameter };
        const Rectangle<float>  constrain { 0.0f, 0.0f, 0.0f, 0.0f };
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

    struct Descriptor 
    {
        const map::module::type type = map::module::type::off;
        const int* const cv[map::cv::count];
        const std::string* const prefix;
        const Control* const set[map::cv::count];
        const Rectangle<float>* const constrain; 
    };
}
