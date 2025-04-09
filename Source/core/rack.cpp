#include "rack.hpp"
#include "uid.hpp"


namespace core
{
    module_t* rack_t::at(const int& pos) 
    { 
        return node.get()[pos]; 
    }

    module_t* rack_t::at(const module_type& t, const int& pos)
    {
        for(int i = 0; i < bus.blueprint.mc; ++i)
        {
            if(node.get()[i]->descriptor->mtype == t)
            {
                if(node.get()[i]->position == pos) return node.get()[i];
            }
        }
        return nullptr;
    }

    
    void rack_t::process(const int& p) 
    { 
        node[p]->process(); 
    }

    void rack_t::bind(module_t* m, const unsigned& pos) 
    { 
        if(pos < bus.blueprint.mc) node[pos] = m; 
    }

    rack_t::rack_t(const descriptor_t* d): bus(d) 
    { 
        allocate(); 
        build(); 
    }

    rack_t::~rack_t() 
    { 
        for(int i = 0; i < bus.blueprint.mc; ++i) delete node[i]; 
    }

    void rack_t::build() noexcept 
    { 
        #ifdef DEBUG 
        std::cout<<"-- Creating modules...\n";
        #endif
        for(int i = 0; i < bus.blueprint.mc; ++i)
        {
            node[i] = create_node(bus.blueprint.descriptor[i].mtype);
        }
    }

    module_t* rack_t::create_node(const module_type& t)
    {
        switch (t)
        {
            case module_type::vco: return new vco_t(); break;
            case module_type::env: return new env_t(); break;
            case module_type::lfo: return new lfo_t(); break;
            case module_type::map: return new map_t(); break;
            case module_type::mix: return new mix_t(); break;
            case module_type::pdt: return new pdt_t(); break;
            case module_type::rtr: return new rtr_t(); break;
            case module_type::snh: return new snh_t(); break;
            case module_type::sum: return new sum_t(); break;
            case module_type::vca: return new vca_t(); break;
            case module_type::vcd: return new vcd_t(); break;
            case module_type::vcf: return new vcf_t(); break;
            
            // case module_type::com: return new lfo_t(); break;
            // case module_type::cro: return new lfo_t(); break;
            // case module_type::fuse: return new lfo_t(); break;

            
            default: break;
        }
        return new lfo_t();
    }

    void rack_t::allocate() noexcept 
    { 
        #ifdef DEBUG 
        std::cout<<"-- Allocating space for rack...\n";
        #endif
        node = std::make_unique<core::module_t*[]>(bus.blueprint.mc); 
    }

    void rack_t::connect_pin_i(const uint32_t& hash, std::atomic<float>** o)
    {
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < bus.blueprint.mc; ++i)
        {
            if(node[i]->descriptor->mtype == _uid.module)
            {
                if(node[i]->position == _uid.module_position)
                {
                    break;
                }
            }
        }
        o = &node[i]->icv[_uid.parameter_position];
    }

    void rack_t::connect_pin_o(const uint32_t& hash, std::atomic<float>* o)
    {
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < bus.blueprint.mc; ++i)
        {
            if(node[i]->descriptor->mtype == _uid.module)
            {
                if(node[i]->position == _uid.module_position)
                {
                    break;
                }
            }
        }
        o = &node[i]->ocv[_uid.parameter_position];
    }

    void rack_t::connect_pin_c(const uint32_t& hash, std::atomic<float>* o)
    {
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < bus.blueprint.mc; ++i)
        {
            if(node[i]->descriptor->mtype == _uid.module)
            {
                if(node[i]->position == _uid.module_position)
                {
                    break;
                }
            }
        }
        node[i]->ccv[_uid.parameter_position] = o;
    }

}
