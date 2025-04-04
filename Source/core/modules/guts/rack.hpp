#pragma once


#include "bus.hpp"
#include "node.hpp"

namespace core
{
    class rack_t
    {
        private:
            std::unique_ptr<core::module*[]>      node;
            void allocate() noexcept;

        public:
            const interface::module_map* const mmap;
            void connect_pin_i(const uint32_t&, std::atomic<float>**);
            void connect_pin_o(const uint32_t&, std::atomic<float>*);
            void connect_pin_c(const uint32_t&, std::atomic<float>*);

            uint32_t get_uid(const module_type&, const int&, const parameter_type&, const int&);
            core::module* at(const int& pos) { return node.get()[pos]; }
            void process(const int& p) { node[p]->process(); }
            void bind(core::module* m, const unsigned& pos) { if(pos < mmap->blueprint->mc) node[pos] = m; }
            rack_t(const interface::module_map* const d): mmap(d) { allocate(); };
           ~rack_t() {};
    };
}
