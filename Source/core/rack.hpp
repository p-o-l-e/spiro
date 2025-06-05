#pragma once

#include "grid.hpp"
#include "node.hpp"
#include "module_headers.hpp"

namespace core
{
    class Rack
    {
        private:
            const Grid* const grid;
            Module<float>** node;
            Module<float>* create_node(const map::module::type&);

        public:
            Module<float>* at(const map::module::type&, const int&) const noexcept;
            Module<float>* at(const int&) const noexcept;
            void process(const int&) noexcept;
            Rack(const Grid*);
           ~Rack() noexcept;
    };

}
