#pragma once
#include <string>
#include <cmath>
#include <map>
#include <atomic>
#include "constants.hpp"
#include "iospecs.hpp"
#include "node.h"

namespace cell {


    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace snh
        {
            const int ctrls  { 1 };
            const int ins    { 3 };
            const int outs   { 1 };

            enum class ctrl { time };
            enum class in   { time, a, b };
            enum class out  { a };

            const std::string prefix {"snh_"};
            const std::map<interface::snh::ctrl, std::string> ctrl_postfix
            {
                { ctrl::time, "_ctrl_time" }
            };
            const std::map<interface::snh::in, std::string> in_postfix
            {
                { in::a, "_in_a" },
                { in::b, "_in_b" }
            };
            const std::map<interface::snh::out, std::string> out_postfix
            {
                { out::a, "_out_a" },
            };
        }
    }

    class snh_t: public module
    {
        private:
            static int idc;
            float t     = 0.0f;
            float value = 0.0f;
            float scale = 40.0f;

        public:
            int id;
            void process() override;
            void reset();

            snh_t();
           ~snh_t() {};
    };

}
