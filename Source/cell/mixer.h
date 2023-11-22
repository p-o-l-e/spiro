#pragma once
#include "node.h"
#include "constants.hpp"
#include "containers.hpp"
#include "utility.hpp"

namespace cell
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace mxr       
        {
            const int ctrls  { 3 };
            const int ins    { 5 };
            const int outs   { 2 };

            enum class ctrl { alpha, theta, volume };
            enum class in   { l, c, r, alpha, theta };
            enum class out  { l, r };

        }
    }

    class mixer: public module
    {
        public:
            void process() override
            {
                lcr<float> a 
                { 
                    in[static_cast<int>(interface::mxr::in::l)]->load(), 
                    in[static_cast<int>(interface::mxr::in::c)]->load(), 
                    in[static_cast<int>(interface::mxr::in::r)]->load() 
                };
                float lc = ctrl[static_cast<int>(interface::mxr::ctrl::alpha)]->load() + in[static_cast<int>(interface::mxr::in::alpha)]->load();
                float cr = ctrl[static_cast<int>(interface::mxr::ctrl::theta)]->load() + in[static_cast<int>(interface::mxr::in::theta)]->load();
                l_r<float> lr = c3c2(a, lc, cr);
                out[static_cast<int>(interface::mxr::out::l)].store(lr.l * ctrl[static_cast<int>(interface::mxr::ctrl::volume)]->load());
                out[static_cast<int>(interface::mxr::out::r)].store(lr.r * ctrl[static_cast<int>(interface::mxr::ctrl::volume)]->load());
            }

            mixer()
            {
                init(interface::mxr::ctrls, interface::mxr::ins, interface::mxr::outs);
            }
           ~mixer() {};
    };

}; // Namespace
