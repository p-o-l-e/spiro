#include "blueprint.hpp"
#include "uid.hpp"
#include <iostream>

namespace core
{
namespace interface
{
    const uint32_t blueprint_t::get_hash(const parameter_type& t, const int& p) const
    {
        switch(t)
        {
            case parameter_type::cc: return hash_table_c[p]; break;
            case parameter_type::ic: return hash_table_i[p]; break;
            case parameter_type::oc: return hash_table_o[p]; break;
            default: break;
        }
        return 0;
    }


    const int blueprint_t::get_index(const uint32_t& hash) const
    {
        switch(static_cast<parameter_type>(core::extract_byte(hash, uid_t::shift::m_index)))
        {
            case parameter_type::cc:
                for(int i = 0; i < cc; ++i) if(hash_table_c[i] == hash) return i;    
                break;
            case parameter_type::ic:
                for(int i = 0; i < ic; ++i) if(hash_table_i[i] == hash) return i;    
                break;
            case parameter_type::oc:
                for(int i = 0; i < oc; ++i) if(hash_table_o[i] == hash) return i;    
                break;
            default:
                break;
        }
        return error::invalid_index;
    }

    const std::unique_ptr<int[]> blueprint_t::set_relatives(const descriptor_t* d) const
    {
        int n = settings::n_modules();
        auto r = std::make_unique<int[]>(n);

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            module_type car = d[s].mtype;
            r[s] = pos;

            for(int i = s + 1; i < n; ++i)
            {
                if(car == d[i].mtype) 
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
            for(int i = 0; i < *descriptor[m].cc; ++i)
            {
                hash_table_c[h] = core::uid_t::encode_uid(descriptor[m].mtype, parameter_type::cc, relative[m], i);
                ++h;
            }
        }
        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].ic; ++i)
            {
                hash_table_i[h] = core::uid_t::encode_uid(descriptor[m].mtype, parameter_type::ic, relative[m], i);
                ++h;
            }
        }
        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].oc; ++i)
            {
                hash_table_o[h] = core::uid_t::encode_uid(descriptor[m].mtype, parameter_type::oc, relative[m], i);
                ++h;
            }
        }
        #ifdef DEBUG 
            std::cout<<"---- Hash calculated...\n";
        #endif
    }


    const int blueprint_t::count(const parameter_type& p, const descriptor_t* d) const
    {
        int c { 0 };
        int m = settings::n_modules();
        switch(p)
        {
            case parameter_type::ic: for(int i = 0; i < m; ++i) c += *d[i].ic; break;
            case parameter_type::oc: for(int i = 0; i < m; ++i) c += *d[i].oc; break;
            case parameter_type::cc: for(int i = 0; i < m; ++i) c += *d[i].cc; break;
            default: break;
        }
        return c;
    }

    blueprint_t::blueprint_t(const descriptor_t* d): 
        descriptor(d), 
        relative(set_relatives(d)),
        mc(settings::n_modules()), 
        ic(count(parameter_type::ic, d)),
        oc(count(parameter_type::oc, d)),
        cc(count(parameter_type::cc, d))
    { 
        calculate_hash();
        #ifdef DEBUG 
            std::cout<<"-- Blueprint created...\n";
        #endif
    };
}
}
