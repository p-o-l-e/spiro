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
        namespace pct
        {
            const int ctrls  { 0 };
            const int ins    { 3 };
            const int outs   { 1 };

            enum class ctrl { };
            enum class in   { a, b, c };
            enum class out  { a };

        }
    }


    class product: public module
    {
        private:
            static int idc;
        public:
            int id;
            void process() override
            {
                bool  o = false;
                float s = 1.0f;
                for(int i = 0; i < interface::pct::ins; ++i)
                {
                    if(in[i] == &zero) continue;
                    else 
                    {
                        s *= in[i]->load();
                        o = true;
                    }
                }
                out[0].store( o ? s : 0.0f);
            };

            product(): id(++idc)
            { 
                init(interface::pct::ctrls, interface::pct::ins, interface::pct::outs, module_type::product, id); 
            };
};

}; // Namespace
