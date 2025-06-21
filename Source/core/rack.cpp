#include "rack.hpp"
#include "modules/interface/descriptor.hxx"
#include <cstdint>
#include <locale>

namespace core
{
    Module<float>* Rack::at(const map::module::type& type, const int& pos) const noexcept
    {
        uint16_t uid { (static_cast<uint8_t>(type) << 8) + pos };
        return moduleMap.find(uid)->second;
    }

    Module<float>* Rack::at(const int& pos) const noexcept
    {
        return node[pos];
    }

    int Rack::index(uint8_t mt, uint8_t mp) const noexcept
    {
        uint16_t uid { (static_cast<uint8_t>(mt) << 8) + mp };
        return indexMap.find(uid)->second;
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
        calculateModuleMap();
        calculateIndexMap();
        std::cout<<"-- Rack built...\n";
    }

    void Rack::calculateModuleMap()
    {
        for(int i = 0; i < grid->sectors; ++i)
        {
            uint16_t uid { (static_cast<uint8_t>(node[i]->descriptor->type) << 8) + node[i]->position }; 
            moduleMap.emplace(uid, node[i]);
        }
    }

    void Rack::calculateIndexMap()
    {
        std::unordered_map<uint16_t, int> r {};
        for(int i = 0; i < grid->sectors; ++i)
        {
            uint16_t uid { (static_cast<uint8_t>(node[i]->descriptor->type) << 8) + node[i]->position }; 
            indexMap.emplace(uid, i);
        }
    }

    Module<float>* Rack::create_node(const map::module::type& t)
    {
        switch (t)
        {
            case map::module::type::env: return new ENV(); break;
            case map::module::type::lfo: return new LFO(); break;
            case map::module::type::cso: return new CSO(); break;
            case map::module::type::mix: return new MIX(); break;
            case map::module::type::pdt: return new PDT(); break;
            case map::module::type::rtr: return new RTR(); break;
            case map::module::type::snh: return new SNH(); break;
            case map::module::type::sum: return new SUM(); break;
            case map::module::type::vca: return new VCA(); break;
            case map::module::type::vcd: return new VCD(); break;
            case map::module::type::vcf: return new VCF(); break;
            case map::module::type::vco: return new VCO(); break;
            case map::module::type::cro: return new CRO(); break;
            case map::module::type::com: return new COM(); break;
            default: break;
        }
        return nullptr;
    }
   
    Rack::~Rack()
    { 
        std::cout<<"Rack::~Rack()\n";
        for(int i = 0; i < grid->sectors; ++i) delete node[i]; 
        delete[] node;
    }
}
