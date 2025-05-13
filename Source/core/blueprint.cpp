#include "blueprint.hpp"
#include "uid.hpp"
#include <iostream>

namespace core
{
namespace interface
{
    const uint32_t Blueprint::get_hash(const core::map::parameter::type& t, const int& p) const
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


    const int Blueprint::get_index(const uint32_t& hash) const
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

    const std::unique_ptr<int[]> Blueprint::set_relatives(const Descriptor* d) const
    {
        int n = settings::modules_n;
        auto r = std::make_unique<int[]>(n);

        for(int s = 0; s < n; ++s)
        {
            int pos = 0;
            core::map::module::type car = d[s].type;
            r[s] = pos;

            for(int i = s + 1; i < n; ++i)
            {
                if(car == d[i].type) 
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

    void Blueprint::calculate_hash()
    {
        hash_table_c = std::make_unique<uint32_t[]>(cc);
        hash_table_i = std::make_unique<uint32_t[]>(ic);
        hash_table_o = std::make_unique<uint32_t[]>(oc);

        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].cv[map::cv::c]; ++i)
            {
                hash_table_c[h] = core::uid_t::encode_uid(descriptor[m].type, core::map::parameter::type::cc, relative[m], i);
                ++h;
            }
        }
        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].cv[map::cv::i]; ++i)
            {
                hash_table_i[h] = core::uid_t::encode_uid(descriptor[m].type, core::map::parameter::type::ic, relative[m], i);
                ++h;
            }
        }
        for(int m = 0, h = 0; m < mc; ++m)
        {
            for(int i = 0; i < *descriptor[m].cv[map::cv::o]; ++i)
            {
                hash_table_o[h] = core::uid_t::encode_uid(descriptor[m].type, core::map::parameter::type::oc, relative[m], i);
                ++h;
            }
        }
        #ifdef DEBUG 
            std::cout<<"---- Hash calculated...\n";
        #endif
    }


    const int Blueprint::count(const core::map::parameter::type& p, const Descriptor* d) const
    {
        int c { 0 };
        switch(p)
        {
            case map::parameter::type::ic: for(int i = 0; i < settings::modules_n; ++i) c += *d[i].cv[map::cv::i]; break;
            case map::parameter::type::oc: for(int i = 0; i < settings::modules_n; ++i) c += *d[i].cv[map::cv::o]; break;
            case map::parameter::type::cc: for(int i = 0; i < settings::modules_n; ++i) c += *d[i].cv[map::cv::c]; break;
            default: break;
        }
        return c;
    }

    Blueprint::Blueprint(const Descriptor* d): 
        descriptor(d), 
        relative(set_relatives(d)),
        mc(settings::modules_n), 
        ic(count(map::parameter::type::ic, d)),
        oc(count(map::parameter::type::oc, d)),
        cc(count(map::parameter::type::cc, d))
    { 
        calculate_hash();
        #ifdef DEBUG 
            std::cout<<"-- Blueprint created...\n";
        #endif
    };
}
}
