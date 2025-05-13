#pragma once

#include "setup/constants.hpp"
#include "setup/map.hpp"
#include "interface_headers.hpp"
#define DEBUG


namespace core {
    enum error { invalid_index = -1 };
namespace interface {

	class Blueprint
    {
        private:
            const int count(const core::map::parameter::type&, const std::span<Descriptor>*) const;
            const std::unique_ptr<int[]> set_relatives(const std::span<Descriptor>*) const;
            void calculate_hash();
            std::unique_ptr<uint32_t[]> hash_table_i; 
            std::unique_ptr<uint32_t[]> hash_table_o;
            std::unique_ptr<uint32_t[]> hash_table_c;

        public:
            const int get_index(const uint32_t&) const;
            const uint32_t get_hash(const core::map::parameter::type&, const int&) const;
            const std::span<Descriptor>* descriptor;
            const std::unique_ptr<int[]> relative; 
            const int ic = 0;
            const int oc = 0;
            const int cc = 0;
            const int mc = 0;

            Blueprint(const std::span<Descriptor>*);
           ~Blueprint() {};
    };
}
} // Namespace core
