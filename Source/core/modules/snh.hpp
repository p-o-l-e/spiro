#pragma once
#include <string>
#include <cmath>
#include <map>
#include <atomic>
#include "constants.hpp"
#include "interface/snh_interface.hpp"
#include "iospecs.hpp"
#include "node.h"

namespace cell {
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
