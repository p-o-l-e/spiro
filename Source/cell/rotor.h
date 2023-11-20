#pragma once
#include "containers.hpp"
#include "constants.hpp"
#include "node.h"

namespace cell
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace rtr
        {
            const int ctrls  { 3 };
            const int ins    { 9 };
            const int outs   { 6 };

            enum class ctrl {  x,  y,  z };
            enum class in   { ax, ay, az, bx, by, bz, cvx, cvy, cvz };
            enum class out  { ax, ay, az, bx, by, bz };

        }
    }

using namespace interface::rtr;

class rotor: public module
{
    public:

        void process() override
        {
            lcr<float> a 
            { 
                in[static_cast<int>(in::ax)]->load() + 
                in[static_cast<int>(in::bx)]->load(),
                in[static_cast<int>(in::ay)]->load() +
                in[static_cast<int>(in::by)]->load(),
                in[static_cast<int>(in::az)]->load() +
                in[static_cast<int>(in::bz)]->load() 
            };

            float x = ctrl[static_cast<int>(ctrl::x)]->load() + pi * in[static_cast<int>(in::cvx)]->load();
            float y = ctrl[static_cast<int>(ctrl::y)]->load() + pi * in[static_cast<int>(in::cvy)]->load();
            float z = ctrl[static_cast<int>(ctrl::z)]->load() + pi * in[static_cast<int>(in::cvz)]->load();
            lcr<float> f = Rotate(a, x, y, z);
            
            out[static_cast<int>(out::ax)].store(f.l);
            out[static_cast<int>(out::ay)].store(f.c);
            out[static_cast<int>(out::az)].store(f.r);

            out[static_cast<int>(out::bx)].store(f.l);
            out[static_cast<int>(out::by)].store(f.c);
            out[static_cast<int>(out::bz)].store(f.r);
        }

        rotor() 
        { 
            init(interface::rtr::ctrls, interface::rtr::ins, interface::rtr::outs);
        };
       ~rotor() {};
};

}
