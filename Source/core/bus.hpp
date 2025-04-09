#pragma once
#include "blueprint.hpp"


namespace core 
{
    namespace interface
    {   
        struct bus_connector
        {
            private:
                std::unique_ptr<std::atomic<float>*[]> _c;
                std::unique_ptr<std::atomic<float>*[]> _i;
                std::unique_ptr<std::atomic<float>*[]> _o;

            public:
                const blueprint_t blueprint;
                std::atomic<float>* pin_c(const uint32_t&) const;
                std::atomic<float>* pin_i(const uint32_t&) const;
                std::atomic<float>* pin_o(const uint32_t&) const;

                bus_connector(const descriptor_t*);
               ~bus_connector() {};
        };
    }
}