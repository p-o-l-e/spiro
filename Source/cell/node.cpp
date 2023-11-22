#include "node.h"

namespace cell
{

    void module::set_uid(const module_type& t, const uint8_t& id)
    {
        int i { 0 };
        int n { 0 };
        uint32_t mi = get_module_index(module_type::vco, id);
        mi <<= 16;
        for(; i < _cs; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::ctrl, n) };
            uid[n] = f;
            ++n;
        }
        for(; i < _is; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::in, n) };
            uid[n] = f;
            ++n;
        }
        for(; i < _os; ++i) 
        {
            uint32_t f { mi + get_parameter_index(parameter_type::out, n) };
            uid[n] = f;
            ++n;
        }
    }

    module::~module() = default;
}
