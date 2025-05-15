#pragma once
#include "node.h"
#include "constants.hpp"

namespace cell
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace sum
        {
            const int ctrls  { 0 };
            const int ins    { 2 };
            const int outs   { 2 };

            enum class ctrl { };
            enum class in   { a, b };
            enum class out  { a, b };

        }
    }

    using namespace interface::sum;

    class sigma: public module
    {
        private:
            static int idc;

        public:
            int id;
            void process() override
            {
                out[0].store(in[0]->load() + in[1]->load());
                out[1].store(out[0]);
            };


            sigma(): id(++idc)
            { 
                init(interface::sum::ctrls, interface::sum::ins, interface::sum::outs, module_type::sigma, id);
            };
           ~sigma() {};
};
}
