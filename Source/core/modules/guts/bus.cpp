#include "bus.hpp"
#include "uid.hpp"
#include <iostream>

namespace core {
namespace interface { 

    const int module_map::get_index(const uint32_t& hash) const
    {
        switch(static_cast<parameter_type>(core::extract_byte(hash, uid_t::shift::m_index)))
        {
            case parameter_type::cc:
                for(int i = 0; i < blueprint->cc; ++i) if(hash_table_c[i] == hash) return i;    
            break;

            case parameter_type::ic:
                for(int i = 0; i < blueprint->ic; ++i) if(hash_table_i[i] == hash) return i;    
            break;

            case parameter_type::oc:
                for(int i = 0; i < blueprint->oc; ++i) if(hash_table_o[i] == hash) return i;    
            break;

            default:
            break;
        }
        return -1;
    }


    const std::unique_ptr<uint32_t[]> module_map::calculate_hash(const parameter_type& p, const blueprint_t* o)
    {
        std::unique_ptr<uint32_t[]> hash;

        switch(p)
        {
            case parameter_type::cc:
                hash = std::make_unique<uint32_t[]>(o->cc);
                for(int m = 0, h = 0; m < o->mc; ++m)
                {
                    for(int i = 0; i < *o->descriptor[m].cc; ++i)
                    {
                        hash[h] = core::uid_t::encode_uid(o->descriptor[m].mtype, p, o->relative[m], i);
                        ++h;
                    }
                }
                break;

            case parameter_type::ic:
                hash = std::make_unique<uint32_t[]>(o->ic);
                for(int m = 0, h = 0; m < o->mc; ++m)
                {
                    for(int i = 0; i < *o->descriptor[m].ic; ++i)
                    {
                        hash[h] = core::uid_t::encode_uid(o->descriptor[m].mtype, p, o->relative[m], i);
                        ++h;
                    }
                }
                break;

            case parameter_type::oc:
                hash = std::make_unique<uint32_t[]>(o->oc);
                for(int m = 0, h = 0; m < o->mc; ++m)
                {
                    for(int i = 0; i < *o->descriptor[m].oc; ++i)
                    {
                        hash[h] = core::uid_t::encode_uid(o->descriptor[m].mtype, p, o->relative[m], i);
                        ++h;
                    }
                }
                break;
            
            default:
                break;
        }
        return hash;
    }

    module_map::module_map(const blueprint_t* d): 
        blueprint(d),
        hash_table_c(calculate_hash(parameter_type::cc, d)),
        hash_table_i(calculate_hash(parameter_type::ic, d)),
        hash_table_o(calculate_hash(parameter_type::oc, d))
    { };

} // namespace interface
} // namespace core


