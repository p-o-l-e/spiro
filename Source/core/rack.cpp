#include "rack.hpp"
#include "uid.hpp"


namespace core
{
    Module* Rack::at(const int& pos) 
    { 
        return node.get()[pos]; 
    }

    Module* Rack::at(const map::module::type& t, const int& pos)
    {
        for(int i = 0; i < bus.blueprint.mc; ++i)
        {
            if(node.get()[i]->descriptor->type == t)
            {
                if(node.get()[i]->position == pos) return node.get()[i];
            }
        }
        return nullptr;
    }

    
    void Rack::process(const int& p) 
    { 
        node[p]->process(); 
    }

    void Rack::bind(Module* m, const unsigned& pos) 
    { 
        if(pos < bus.blueprint.mc) node[pos] = m; 
    }

    Rack::Rack(const Descriptor** d): bus(d) 
    { 
        allocate(); 
        build(); 
    }

    Rack::~Rack() 
    { 
        for(int i = 0; i < bus.blueprint.mc; ++i) delete node[i]; 
    }

    void Rack::build() noexcept 
    { 
        LOG("Rack::build() : ");
        for(int i = 0; i < bus.blueprint.mc; ++i)
        {
            node[i] = create_node(bus.blueprint.descriptor[i]->type);
        }
    }

    Module* Rack::create_node(const map::module::type& t)
    {
        switch (t)
        {
            case map::module::type::vco: return new vco_t(); break;
            case map::module::type::env: return new env_t(); break;
            case map::module::type::lfo: return new lfo_t(); break;
            case map::module::type::cso: return new cso_t(); break;
            case map::module::type::mix: return new mix_t(); break;
            case map::module::type::pdt: return new pdt_t(); break;
            case map::module::type::rtr: return new rtr_t(); break;
            case map::module::type::snh: return new snh_t(); break;
            case map::module::type::sum: return new sum_t(); break;
            case map::module::type::vca: return new vca_t(); break;
            case map::module::type::vcd: return new vcd_t(); break;
            case map::module::type::vcf: return new vcf_t(); break;
            
            // case map::module::type::com: return new lfo_t(); break;
            // case map::module::type::cro: return new lfo_t(); break;
            // case map::module::type::fuse: return new lfo_t(); break;

            
            default: break;
        }
        return new lfo_t();
    }

    void Rack::allocate() noexcept 
    { 
        LOG("Rack::allocate() :"); 
        node = std::make_unique<Module*[]>(bus.blueprint.mc); 
        LOG("-- Space for rack allocated...");
    }

    void Rack::connect_pin_i(const uint32_t& hash, std::atomic<float>** o)
    {
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < bus.blueprint.mc; ++i)
        {
            if(node[i]->descriptor->type == _uid.mt)
            {
                if(node[i]->position == _uid.mp)
                {
                    break;
                }
            }
        }
        o = &node[i]->icv[_uid.pp];
    }

    void Rack::connect_pin_o(const uint32_t& hash, std::atomic<float>* o)
    {
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < bus.blueprint.mc; ++i)
        {
            if(node[i]->descriptor->type == _uid.mt)
            {
                if(node[i]->position == _uid.mp)
                {
                    break;
                }
            }
        }
        o = &node[i]->ocv[_uid.pp];
    }

    void Rack::connect_pin_c(const uint32_t& hash, std::atomic<float>* o)
    {
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < bus.blueprint.mc; ++i)
        {
            if(node[i]->descriptor->type == _uid.mt)
            {
                if(node[i]->position == _uid.mp)
                {
                    break;
                }
            }
        }
        node[i]->ccv[_uid.pp] = o;
    }

}
