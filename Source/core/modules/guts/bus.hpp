#pragma once
#include "../setup/blueprint.hpp"


namespace core 
{

namespace interface
{
    /*
    * Creates and Stores hash tables for controls and ports
    */
	class module_map
    {
        public:
            // Hash with corresponding global indices for bus_connector
            const std::unique_ptr<uint32_t[]> hash_table_i; 
            const std::unique_ptr<uint32_t[]> hash_table_o;
            const std::unique_ptr<uint32_t[]> hash_table_c;
        private:
            const std::unique_ptr<uint32_t[]> calculate_hash(const parameter_type&, const blueprint_t*);


        public:
            const blueprint_t* blueprint;
            const int get_index(const uint32_t& hash) const;

            module_map(const blueprint_t* d);
           ~module_map() {};
    };
    
	struct bus_connector
	{
        private:
            const module_map* const mmap;
		    std::unique_ptr<std::atomic<float>*[]> _c;
            std::unique_ptr<std::atomic<float>*[]> _i;
            std::unique_ptr<std::atomic<float>*[]> _o;

        public:
            std::atomic<float>* pin_c(const uint32_t& c) { return _c[mmap->get_index(c)]; }
            std::atomic<float>* pin_i(const uint32_t& i) { return _i[mmap->get_index(i)]; }
            std::atomic<float>* pin_o(const uint32_t& o) { return _o[mmap->get_index(o)]; }

            bus_connector(const module_map* r): mmap(r)
            {
                for(int i = 0; i < mmap->blueprint->cc; ++i) _c[i] = &core::zero;
                for(int i = 0; i < mmap->blueprint->ic; ++i) _i[i] = &core::zero;
                for(int i = 0; i < mmap->blueprint->oc; ++i) _o[i] = &core::zero;
            }
            ~bus_connector() {};
	};
}
}