#pragma once

#include <atomic>
#include <iostream>
#include "constants.hpp"
#include <cstdint>
#include <ios>
#include <sys/types.h>
#include <unordered_map>
#include "grid.hpp"

namespace cell 
{
    enum class parameter_type { cc = 0xFF, ic = 0xEE, oc = 0xDD };

    struct uid_t;  
    constexpr uint16_t get_module_index(const module_type& t, const uint8_t& id);
    constexpr uint16_t get_parameter_index(const parameter_type& t, const uint8_t& id);
    constexpr uint32_t encode_uid(const uid_t&) noexcept;
    uid_t decode_uid(const uint32_t&) noexcept;

    struct uid_t {
        module_type module = module_type::fuse;
        int module_position = 0;                       
        parameter_type parameter;
        int parameter_position = 0;
        uint32_t hash = 0;
    };


    namespace settings 
    {
        const int  parameter_types = 3;
 
        inline const unsigned osc_n 	{ 4 }; // # Oscillators
        inline const unsigned lfo_n     { 2 }; // # LFOs
        inline const unsigned map_n     { 2 }; // # CSOs
        inline const unsigned snh_n     { 2 }; // # SNHs
        inline const unsigned env_n 	{ 4 }; // # Envelopes
        inline const unsigned vcf_n 	{ 2 }; // # Filters
        inline const unsigned vca_n 	{ 2 }; // # VCAs
        inline const unsigned vcd_n     { 1 }; // # Delays
        inline const unsigned rtr_n     { 1 }; // # Rotors
        inline const unsigned sum_n     { 2 }; // # Summators
        inline const unsigned pdt_n     { 1 }; // # Multipliers
        inline const unsigned mix_n     { 1 }; // # Mixers
        inline const unsigned ctr_n 	{ 5 }; // # Controls
        inline const unsigned mod_n 	{ 1 }; // # Modulators


        inline const unsigned prm_n 	{ 82 }; // # Parameters
        inline const unsigned btn_n 	{ 23 }; // # Buttons
        inline const unsigned pot_n 	{ 50 }; // # Potentiometers
        inline const unsigned ports_in 	{ 77 }; // # Inputs
        inline const unsigned ports_out { 47 }; // # Outputs
        
        inline const unsigned ports_n 	    { ports_in + ports_out };
        inline const unsigned controls_n    { prm_n + btn_n + pot_n }; 


        inline const descriptor_t descriptor_map[settings::module_types]
        {
            { interface::vco::descriptor },
            { interface::env::descriptor[0] },
            { interface::lfo::descriptor },
            { interface::map::descriptor },
            { interface::mix::descriptor },
            { interface::pdt::descriptor },
            { interface::rtr::descriptor },
            { interface::snh::descriptor[0] },
            { interface::sum::descriptor },
            { interface::vca::descriptor[0] },
            { interface::vcd::descriptor },
            { interface::vcf::descriptor }
        };



    }


namespace interface
{
	class registry_t
    {
        private:
            const descriptor_t* module;
            const std::unique_ptr<uint32_t[]> hash_table_i;
            const std::unique_ptr<uint32_t[]> hash_table_o;
            const std::unique_ptr<uint32_t[]> hash_table_c;

			const int in = 0;
			const int on = 0;
			const int cn = 0;

            const int mn = 0;             // Modules N

			void fill_lists();

        public:
            constexpr uint32_t get_hash(const parameter_type&, const int&) const;
            constexpr int get_index(const uint32_t& hash) const;
            constexpr int modules() const   { return mn; };
            constexpr int inputs() const    { return in; };
            constexpr int outputs() const   { return on; };
            constexpr int controls() const  { return cn; };

            registry_t(const descriptor_t* d): module(d) { fill_lists(); };
           ~registry_t() {};
    };
    
    const registry_t registry(cell::settings::descriptor_map);


	struct bus_connector
	{
        private:
            const registry_t* const _registry;
		    std::atomic<float>* _port[settings::controls_n];
            std::atomic<float>* _grid[settings::ports_in * settings::ports_out];


        public:
            std::atomic<float>* port_pin(const uint32_t& o)
            {
                return _port[_registry->get_index(o)];
            }

            std::atomic<float>* grid_pin(const uint32_t& x, const uint32_t& y)
            {
                return _grid[_registry->get_index(x) * _registry->get_index(y)];
            }
        
            bus_connector(const registry_t* r): _registry(r)
            {
                for(int i = 0; i < _registry->controls(); ++i) 
                {
                    _port[i] = &cell::zero;
                }

            }
            ~bus_connector() {};
	};

}


} // Namespace cell
