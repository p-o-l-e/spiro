#pragma once
#include "node.h"
#include "constants.hpp"
#include "containers.hpp"
#include "interface/mix_interface.hpp"
#include "utility.hpp"

namespace cell
{
    class mixer: public module
    {
        private:
            static int idc;

        public:
            int id;
            void process() override
            {
                point3d<float> a 
                { 
                    in[static_cast<int>(interface::mix::cvi::l)]->load(), 
                    in[static_cast<int>(interface::mix::cvi::c)]->load(), 
                    in[static_cast<int>(interface::mix::cvi::r)]->load() 
                };
                float lc = ctrl[static_cast<int>(interface::mix::ctl::alpha)]->load() + in[static_cast<int>(interface::mix::cvi::alpha)]->load();
                float cr = ctrl[static_cast<int>(interface::mix::ctl::theta)]->load() + in[static_cast<int>(interface::mix::cvi::theta)]->load();
                point2d<float> lr = c3c2(a, lc, cr);
                out[static_cast<int>(interface::mix::cvo::l)].store(lr.x * ctrl[static_cast<int>(interface::mix::ctl::amp)]->load());
                out[static_cast<int>(interface::mix::cvo::r)].store(lr.y * ctrl[static_cast<int>(interface::mix::ctl::amp)]->load());
            }

            mixer(): id(++idc)
            {
                init(id, &interface::mix::descriptor);
            }
           ~mixer() {};
    };

}; // Namespace
