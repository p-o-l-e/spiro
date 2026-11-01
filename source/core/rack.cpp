#include "rack.hpp"
#include "modules/interface/descriptor.hxx"
#include <cstdint>
#include <locale>

namespace core
{
    Module<float>* Rack::at(map::module::type type, uint8_t pos) const noexcept
    {
        uint16_t uid = static_cast<uint8_t>(type) << 8;
        uid += pos;
        return moduleMap.find(uid)->second;
    }

    Module<float>* Rack::at(int pos) const noexcept
    {
        return node[pos];
    }

    int Rack::index(uint8_t mt, uint8_t mp) const noexcept
    {
        uint16_t uid = (mt << 8) + mp;
        return indexMap.find(uid)->second;
    }

    void Rack::process(int p) noexcept 
    { 
        node[p]->process(); 
    }

    Rack::Rack(const Grid* grid): grid(grid)
    { 
        node = new Module<float>*[grid->sectors]; 
        for(int i = 0; i < grid->sectors; ++i)
        {
            node[i] = create_node(grid->sector[i].descriptor->type);
            node[i]->position = grid->getRelative(i);
        }
        calculateModuleMap();
        calculateIndexMap();
    }

    void Rack::calculateModuleMap()
    {
        for(int i = 0; i < grid->sectors; ++i)
        {
            uint16_t uid = static_cast<uint8_t>(node[i]->descriptor->type) << 8;
            uid += node[i]->position ; 
            moduleMap.emplace(uid, node[i]);
        }
    }

    void Rack::calculateIndexMap()
    {
        std::map<uint16_t, int> r {};
        for(int i = 0; i < grid->sectors; ++i)
        {
            uint16_t uid = static_cast<uint8_t>(node[i]->descriptor->type) << 8;
            uid += node[i]->position; 
            indexMap.emplace(uid, i);
        }
    }

    Module<float>* Rack::create_node(map::module::type t)
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
        for(int i = 0; i < grid->sectors; ++i) delete node[i]; 
        delete[] node;
    }
}
