#include "rack.hpp"
#include "descriptor.hxx"
#include "grid.hpp"
#include "uid.hpp"


namespace core
{
    Module* Rack::at(const map::module::type& type, const int& pos) const noexcept
    {
        for(int i = 0; i < grid->sectors; ++i)
        {
            if(node[i]->descriptor->type == type)
            {
                std::cout<<"Type: "<<node[i]->descriptor->type<<" - Position : "<<node[i]->position<<"\n";
                if(node[i]->position == pos) return node[i];
            }
        }
        return nullptr;
    }

    void Rack::process(const int& p) noexcept 
    { 
        node[p]->process(); 
    }

    Rack::Rack(const Grid* grid): grid(grid), bus(grid) 
    { 
        std::cout<<"Rack::Rack()\n"; 
        node = new Module*[grid->sectors]; 
        std::cout<<"-- Space for rack allocated...\n";
        for(int i = 0; i < grid->sectors; ++i)
        {
            node[i] = create_node(grid->sector[i].descriptor->type);
            std::cout<<"---- Node type : "<<std::hex<<node[i]->descriptor->type<<"\n";

        }
        std::cout<<"-- Rack built...\n";
    }

    Module* Rack::create_node(const map::module::type& t)
    {
        switch (t)
        {
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
            case map::module::type::vco: return new vco_t(); break;
            case map::module::type::cro: return new cro_t(); break;
            default: break;
        }
        return nullptr;
    }
   
    Rack::~Rack() noexcept
    { 
        std::cout<<"Rack::~Rack()\n";
        for(int i = 0; i < grid->sectors; ++i) delete node[i]; 
        delete[] node;
    }
    
    void Rack::connectInput(const uint32_t& hash, std::atomic<float>** o)
    {
        // uid_t _uid = uid_t(hash);
        // int i = 0;
        // for(; i < bus.blueprint->sectors; ++i)
        // {
        //     if(node[i]->descriptor->type == _uid.mt)
        //     {
        //         if(node[i]->position == _uid.mp)
        //         {
        //             break;
        //         }
        //     }
        // }
        // o = &node[i]->icv[_uid.pp];
    }

    void Rack::connectOutput(const uint32_t& hash, std::atomic<float>* o)
    {
        // uid_t _uid = uid_t(hash);
        // int i = 0;
        // for(; i < bus.blueprint.mc; ++i)
        // {
        //     if(node[i]->descriptor->type == _uid.mt)
        //     {
        //         if(node[i]->position == _uid.mp)
        //         {
        //             break;
        //         }
        //     }
        // }
        // o = &node[i]->ocv[_uid.pp];
    }

    void Rack::connectControl(const uint32_t& hash, std::atomic<float>* o)
    {
        // uid_t _uid = uid_t(hash);
        // int i = 0;
        // for(; i < bus.blueprint.mc; ++i)
        // {
        //     if(node[i]->descriptor->type == _uid.mt)
        //     {
        //         if(node[i]->position == _uid.mp)
        //         {
        //             break;
        //         }
        //     }
        // }
        // node[i]->ccv[_uid.pp] = o;
    }

}
