#pragma once
#include "node.h"
#include "interface/sum_interface.hpp"
#include "constants.hpp"

namespace cell
{
    using namespace interface;
    class sigma: public module
    {
        private:
            static int idc;

        public:
            int id;
            void process() override
            {
                out[sum::cvo::a].store(in[sum::cvi::a]->load() + in[sum::cvi::b]->load());
                out[sum::cvo::b].store(out[sum::cvo::a]);
            };


            sigma(): id(++idc)
            { 
                init(id, &sum::descriptor);
            };
           ~sigma() {};
};
}
