#pragma once
#include "node.h"
#include "interface/pdt_interface.hpp"
#include "constants.hpp"

namespace cell
{
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
                for(int i = 0; i < interface::pdt::ic; ++i)
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
                init(id, &interface::pdt::descriptor); 
            };
};

}; // Namespace
