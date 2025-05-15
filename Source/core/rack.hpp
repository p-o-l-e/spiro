#pragma once
#ifdef DEBUG_MODE
    #include <iostream>
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
    #define LOG(x)
#endif

#include "bus.hpp"
#include "node.hpp"
#include "module_headers.hpp"

namespace core
{
    class Rack
    {
        private:
            std::unique_ptr<Module*[]> node;
            Module* create_node(const map::module::type&);
            void allocate() noexcept;
            void build() noexcept;

        public:
            const interface::bus_connector bus;
            void connect_pin_i(const uint32_t&, std::atomic<float>**);
            void connect_pin_o(const uint32_t&, std::atomic<float>*);
            void connect_pin_c(const uint32_t&, std::atomic<float>*);

            uint32_t get_uid(const map::module::type&, const int&, const map::cv::index&, const int&);
            Module* at(const int&);
            Module* at(const map::module::type&, const int&);
            void process(const int&);
            void bind(Module*, const unsigned&);
            Rack(const Descriptor**);
           ~Rack();
    };


}
