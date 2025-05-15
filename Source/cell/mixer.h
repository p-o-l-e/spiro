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
        private:
            static int idc;

        public:
            int id;
            void process() override
            {
                point3d<float> a 
                { 
                    in[static_cast<int>(interface::mxr::in::l)]->load(), 
                    in[static_cast<int>(interface::mxr::in::c)]->load(), 
                    in[static_cast<int>(interface::mxr::in::r)]->load() 
                };
                float lc = ctrl[static_cast<int>(interface::mxr::ctrl::alpha)]->load() + in[static_cast<int>(interface::mxr::in::alpha)]->load();
                float cr = ctrl[static_cast<int>(interface::mxr::ctrl::theta)]->load() + in[static_cast<int>(interface::mxr::in::theta)]->load();
                point2d<float> lr = c3c2(a, lc, cr);
                out[static_cast<int>(interface::mxr::out::l)].store(lr.x * ctrl[static_cast<int>(interface::mxr::ctrl::volume)]->load());
                out[static_cast<int>(interface::mxr::out::r)].store(lr.y * ctrl[static_cast<int>(interface::mxr::ctrl::volume)]->load());
            }

            mixer(): id(++idc)
            {
                init(interface::mxr::ctrls, interface::mxr::ins, interface::mxr::outs, module_type::mixer, id);
            }
           ~mixer() {};
    };

}; // Namespace
