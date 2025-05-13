#pragma once
#include <cstdint>
#include "descriptor.hxx"

namespace core
{


    constexpr uint8_t extract_byte(const uint32_t&, const int&) noexcept;
    struct uid_t 
    {
            enum shift { p_position = 0, p_index, m_position, m_index };

            static const uint32_t encode_uid(const core::map::module::type&, const core::map::parameter::type&, const int&, const int&) noexcept;
            static const uid_t decode_uid(const uint32_t&) noexcept;
            const core::map::module::type module { core::map::module::type::fuse };                      
            const core::map::parameter::type parameter { core::map::parameter::type::off };
            const int module_position { 0 }; 
            const int parameter_position { 0 };
            const uint32_t hash { 0 };

            uid_t() {};
            uid_t(const uint32_t&);
            uid_t(const map::module::type&, const map::parameter::type&, const int&, const int&);
           ~uid_t() {};

    };


    /**************************************************************************************************************************
     * Returns 8-bit value
     * Data : 0x FF FF FF FF
     * index:     3  2  1  0     MSB->LSB
     * ***********************************************************************************************************************/
    constexpr uint8_t extract_byte(const uint32_t& data, const int& index) noexcept
    {
        return uint8_t((data >> (8 * (index % 4))) & 0xFF);
    }
}; 
