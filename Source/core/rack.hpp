#pragma once
#include "grid.hpp"
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
            const Grid* const grid;
            Module** node;
            Module* create_node(const map::module::type&);

        public:
            const interface::bus_connector bus;
            void connectInput(const uint32_t&, std::atomic<float>**);
            void connectOutput(const uint32_t&, std::atomic<float>*);
            void connectControl(const uint32_t&, std::atomic<float>*);
            
            const int ccv(const map::cv::index& i) const { /*return bus.blueprint.ccv[i];*/ };              // Get CV[type] count

            Module* at(const map::module::type&, const int&) const noexcept;
            void process(const int&) noexcept;
            Rack(const Grid*);
           ~Rack() noexcept;
    };


}
