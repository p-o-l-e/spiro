#include "rack.hpp"
#include "descriptor.hxx"
#include "grid.hpp"
#include "uid.hpp"

namespace core
{
    Module<float>* Rack::at(const map::module::type& type, const int& pos) const noexcept
    {
        for(int i = 0; i < grid->sectors; ++i)
        {
            if(node[i]->descriptor->type == type)
            {
                if(node[i]->position == pos) return node[i];
            }
        }
        return nullptr;
    }

    Module<float>* Rack::at(const int& pos) const noexcept
    {
        return node[pos];
    }

    void Rack::process(const int& p) noexcept 
    { 
        node[p]->process(); 
    }

    Rack::Rack(const Grid* grid): grid(grid)
    { 
        std::cout<<"Rack::Rack()\n"; 
        node = new Module<float>*[grid->sectors]; 
        std::cout<<"-- Space for rack allocated...\n";
        for(int i = 0; i < grid->sectors; ++i)
        {
            node[i] = create_node(grid->sector[i].descriptor->type);

        }
        std::cout<<"-- Rack built...\n";
    }

    Module<float>* Rack::create_node(const map::module::type& t)
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
}
