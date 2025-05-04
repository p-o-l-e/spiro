#include "blueprint.hpp"
#include "uid.hpp"
#include <iostream>

namespace core
{
namespace interface
{
    const uint32_t blueprint_t::get_hash(const core::map::parameter::type& t, const int& p) const
    {
        switch(t)
        {
            case core::map::parameter::type::cc: return hash_table_c[p]; break;
            case core::map::parameter::type::ic: return hash_table_i[p]; break;
            case core::map::parameter::type::oc: return hash_table_o[p]; break;
            default: break;
        }
        return 0;
    }


    const int blueprint_t::get_index(const uint32_t& hash) const
    {
        switch(static_cast<core::map::parameter::type>(core::extract_byte(hash, uid_t::shift::m_index)))
        {
            case core::map::parameter::type::cc:
                for(int i = 0; i < cc; ++i) if(hash_table_c[i] == hash) return i;    
                break;
            case core::map::parameter::type::ic:
                for(int i = 0; i < ic; ++i) if(hash_table_i[i] == hash) return i;    
                break;
            case core::map::parameter::type::oc:
                for(int i = 0; i < oc; ++i) if(hash_table_o[i] == hash) return i;    
                break;
            default:
                break;
        }
        return error::invalid_index;
    }

    const std::unique_ptr<int[]> blueprint_t::set_relatives(const std::span<descriptor_t>* d) const
    {
        int n = d->size();
        auto r = std::make_unique<int[]>(n);

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            core::map::module::type car = d[s].data()->type;
            r[s] = pos;

            for(int i = s + 1; i < n; ++i)
            {
                if(car == d[i].data()->type) 
                {
                    r[i] = ++pos;
                    ++s;
                }
            }
        }
        #ifdef DEBUG 
            std::cout<<"---- Relatives set...\n";
        #endif
        return r;
    }

    void blueprint_t::calculate_hash()
    {
        hash_table_c = std::make_unique<uint32_t[]>(cc);
        hash_table_i = std::make_unique<uint32_t[]>(ic);
        hash_table_o = std::make_unique<uint32_t[]>(oc);

        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].data()->cc; ++i)
            {
                hash_table_c[h] = core::uid_t::encode_uid(descriptor[m].data()->type, core::map::parameter::type::cc, relative[m], i);
                ++h;
            }
        }
        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].data()->ic; ++i)
            {
                hash_table_i[h] = core::uid_t::encode_uid(descriptor[m].data()->type, core::map::parameter::type::ic, relative[m], i);
                ++h;
            }
        }
        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].data()->oc; ++i)
            {
                hash_table_o[h] = core::uid_t::encode_uid(descriptor[m].data()->type, core::map::parameter::type::oc, relative[m], i);
                ++h;
            }
        }
        #ifdef DEBUG 
            std::cout<<"---- Hash calculated...\n";
        #endif
    }


    const int blueprint_t::count(const core::map::parameter::type& p, const std::span<descriptor_t>* d) const
    {
        int c { 0 };
        switch(p)
        {
            case core::map::parameter::type::ic: for(auto o: *d) c += *o.ic; break;
            case core::map::parameter::type::oc: for(auto o: *d) c += *o.oc; break;
            case core::map::parameter::type::cc: for(auto o: *d) c += *o.cc; break;
            default: break;
        }
        return c;
    }

    blueprint_t::blueprint_t(const std::span<descriptor_t>* d): 
        descriptor(d), 
        relative(set_relatives(d)),
        mc(d->size()), 
        ic(count(core::map::parameter::type::ic, d)),
        oc(count(core::map::parameter::type::oc, d)),
        cc(count(core::map::parameter::type::cc, d))
    { 
        calculate_hash();
        #ifdef DEBUG 
            std::cout<<"-- Blueprint created...\n";
        #endif
    };
}
}
