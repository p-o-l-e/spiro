#pragma once
#include "grid.hpp"
#include "module_headers.hpp"
#include "modules/node.hpp"
#include <cstdint>
#include <map>

namespace core
{
    class Rack
    {
        private:
            const Grid* const grid;
            std::map<uint16_t, Module<float>*> moduleMap;
            std::map<uint16_t, int> indexMap;
            void calculateModuleMap();
            void calculateIndexMap();
            Module<float>** node;
            Module<float>* create_node(map::module::type);

        public:
            [[nodiscard]] Module<float>* at(map::module::type, uint8_t) const noexcept;
            [[nodiscard]] Module<float>* at(int) const noexcept;
            [[nodiscard]] int index(uint8_t, uint8_t) const noexcept;
            void process(int) noexcept;
            Rack(const Grid*);
           ~Rack();
    };

}
