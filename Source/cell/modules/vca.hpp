#pragma once

#include "constants.hpp"
#include "interface/vca_interface.hpp"
#include "node.h"

namespace cell
{
    using namespace interface;
    class vca_t: public module
    {
        private:
            static int idc;

        public:
            const int id = 0;
            void process() override
            {
                float v = ctrl[vca::ctl::amp]->load() + in[vca::ctl::amp]->load();
                if      (v < 0.0f) v = 0.0f;
                else if (v > 1.0f) v = 1.0f;
                float o = (in[vca::cvi::a]->load() + in[vca::cvi::b]->load()) * v; 
                out[vca::cvo::a].store(o);
                out[vca::cvo::b].store(o);
            };

            vca_t(): id(++idc)
            {  
                init(id, &vca::descriptor[0]);
            };
           ~vca_t() {};
    };

}
