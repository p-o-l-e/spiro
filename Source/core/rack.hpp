#pragma once
#include "grid.hpp"
#include "module_headers.hpp"
#include "modules/node.hpp"
#include <cstdint>
#include <unordered_map>

namespace core
{
    class Rack
    {
        private:
            const Grid* const grid;
            std::unordered_map<uint16_t, Module<float>*> moduleMap;
            std::unordered_map<uint16_t, int> indexMap;
            void calculateModuleMap();
            void calculateIndexMap();
            Module<float>** node;
            Module<float>* create_node(const map::module::type&);

        public:
            Module<float>* at(const map::module::type&, const int&) const noexcept;
            Module<float>* at(const int&) const noexcept;
            int index(uint8_t, uint8_t) const noexcept;
            void process(const int&) noexcept;
            Rack(const Grid*);
           ~Rack();
    };

}
