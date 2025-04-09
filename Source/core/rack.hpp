#pragma once
#define DEBUG
#include <iostream>

#include "bus.hpp"
#include "modules/node.hpp"
#include "module_headers.hpp"

namespace core
{
    class rack_t
    {
        private:
            std::unique_ptr<module_t*[]> node;
            module_t* create_node(const module_type&);
            void allocate() noexcept;
            void build() noexcept;

        public:
            const interface::bus_connector bus;
            void connect_pin_i(const uint32_t&, std::atomic<float>**);
            void connect_pin_o(const uint32_t&, std::atomic<float>*);
            void connect_pin_c(const uint32_t&, std::atomic<float>*);

            uint32_t get_uid(const module_type&, const int&, const parameter_type&, const int&);
            module_t* at(const int&);
            module_t* at(const module_type&, const int&);
            void process(const int&);
            void bind(module_t*, const unsigned&);
            rack_t(const descriptor_t*);
           ~rack_t();
    };


}
