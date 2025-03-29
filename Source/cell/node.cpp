#include "node.h"
#include <cstdint>
#include <sys/types.h>

namespace cell
{

    void module::set_uid(const module_type& t, const uint8_t& id)
    {
        int n { 0 };
        uint32_t mi = get_module_index(t, id);
        mi <<= 16;
        for(int i = 0; i < desc->cc; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::cc, n) };
            uid[n] = f;
            ++n;
        }
        for(int i = 0; i < desc->ic; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::ic, n) };
            uid[n] = f;
            ++n;
        }
        for(int i = 0; i < desc->oc; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::oc, n) };
            uid[n] = f;
            ++n;
        }
    }

    module::~module() = default;



    void rack_t::allocate() noexcept 
    { 
        node = std::make_unique<module*[]>(registry->size()); 
    }

    void rack_t::connect_pin_i(const uint32_t& hash, std::atomic<float>** o)
    {
        uid_t _uid = decode_uid(hash);
        int i = 0;
        for(; i < registry->size(); ++i)
        {
            if(node[i]->desc->mtype == _uid.module)
            {
                if(node[i]->position == _uid.module_position)
                {
                    break;
                }
            }
        }
        o = &node[i]->in[_uid.parameter_position];

    }

    void rack_t::connect_pin_o(const uint32_t& hash, std::atomic<float>* o)
    {
        uid_t _uid = decode_uid(hash);
        int i = 0;
        for(; i < registry->size(); ++i)
        {
            if(node[i]->desc->mtype == _uid.module)
            {
                if(node[i]->position == _uid.module_position)
                {
                    break;
                }
            }
        }
        o = &node[i]->out[_uid.parameter_position];
    }

    void rack_t::connect_pin_c(const uint32_t& hash, std::atomic<float>* o)
    {
        uid_t _uid = decode_uid(hash);
        int i = 0;
        for(; i < registry->size(); ++i)
        {
            if(node[i]->desc->mtype == _uid.module)
            {
                if(node[i]->position == _uid.module_position)
                {
                    break;
                }
            }
        }
        node[i]->ctrl[_uid.parameter_position] = o;
    }

}
