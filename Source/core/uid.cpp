#include "uid.hpp"

namespace core
{
    /**************************************************************************************************************************
     * Returns 32-bit hash [ AA BB CC DD  ]
     * AA : Module type
     * BB : Module id/position
     * CC : Parameter type
     * DD : Parameter id/position
     * ***********************************************************************************************************************/

     const uint32_t uid_t::encode_uid(const module_type& mt, const parameter_type& pt, const int& mp, const int& pp) noexcept
     {
         uint32_t hash = static_cast<uint8_t>(mt);
         hash <<= 8;
         hash += mp;
         hash <<= 8;
         hash += static_cast<uint8_t>(pt);
         hash <<= 8;
         hash += pp;
 
         return hash;
     }

    const uid_t uid_t::decode_uid(const uint32_t& data) noexcept
    {
        return uid_t
        (
            static_cast<module_type>(extract_byte(data, m_index)),
            static_cast<parameter_type>(extract_byte(data, p_index)),
            extract_byte(data, m_position),
            extract_byte(data, p_position)
        );
    }

 
     uid_t::uid_t(const uint32_t& data):
         hash(data),
         module(static_cast<module_type>(extract_byte(data, m_index))),
         module_position(extract_byte(data, m_position)),
         parameter(static_cast<parameter_type>(extract_byte(data, p_index))),
         parameter_position(extract_byte(data, p_position))
     {};
  
     uid_t::uid_t(const module_type& mt, const parameter_type& pt, const int& mp, const int& pp):
         module(mt), parameter(pt), module_position(mp), parameter_position(pp),
         hash(encode_uid(mt, pt, mp, pp))
     {};

}