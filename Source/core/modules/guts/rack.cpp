#include "rack.hpp"
#include "uid.hpp"
#include <cstdint>
#include <sys/types.h>

namespace core
{
    void rack_t::allocate() noexcept 
    { 
        node = std::make_unique<core::module*[]>(mmap->blueprint->mc); 
    }

    void rack_t::connect_pin_i(const uint32_t& hash, std::atomic<float>** o)
    {
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < mmap->blueprint->mc; ++i)
        {
            if(node[i]->get_type() == _uid.module)
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
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < mmap->blueprint->mc; ++i)
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
        uid_t _uid = uid_t(hash);
        int i = 0;
        for(; i < mmap->blueprint->mc; ++i)
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
