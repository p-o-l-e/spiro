#pragma once
#include "containers.hpp"
#include "constants.hpp"
#include "node.h"

namespace cell
{

class rotor: public module
{
    private:
        static int idc;

    public:
        int id;
        quaternion q;

        void process() override
        {
            point3d<float> a 
            { 
                in[static_cast<int>(interface::rtr::cvi::ax)]->load() + 
                in[static_cast<int>(interface::rtr::cvi::bx)]->load(),
                in[static_cast<int>(interface::rtr::cvi::ay)]->load() +
                in[static_cast<int>(interface::rtr::cvi::by)]->load(),
                in[static_cast<int>(interface::rtr::cvi::az)]->load() +
                in[static_cast<int>(interface::rtr::cvi::bz)]->load() 
            };

            float x = ctrl[static_cast<int>(interface::rtr::ctl::x)]->load() + pi * in[static_cast<int>(interface::rtr::cvi::cvx)]->load();
            float y = ctrl[static_cast<int>(interface::rtr::ctl::y)]->load() + pi * in[static_cast<int>(interface::rtr::cvi::cvy)]->load();
            float z = ctrl[static_cast<int>(interface::rtr::ctl::z)]->load() + pi * in[static_cast<int>(interface::rtr::cvi::cvz)]->load();

            q.from_euler(x, y, z);
            q.rotate_vector(a.x, a.y, a.z);

            out[static_cast<int>(interface::rtr::cvo::ax)].store(a.x);
            out[static_cast<int>(interface::rtr::cvo::ay)].store(a.y);
            out[static_cast<int>(interface::rtr::cvo::az)].store(a.z);

            out[static_cast<int>(interface::rtr::cvo::bx)].store(a.x);
            out[static_cast<int>(interface::rtr::cvo::by)].store(a.y);
            out[static_cast<int>(interface::rtr::cvo::bz)].store(a.z);

        }

        rotor(): id(++idc)
        { 
            init(id, &interface::rtr::descriptor);
        };
       ~rotor() {};
};

}
