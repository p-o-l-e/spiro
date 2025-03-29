#include "interface.h"
namespace cell
{
    /**************************************************************************************************************************
     * Returns 16-bit index [ MSB LSB ]
     * MSB : Module type
     * LSB : Module id/position
     * ***********************************************************************************************************************/
    constexpr uint16_t get_module_index(const cell::module_type& t, const uint8_t& id)
    {
        uint16_t r = static_cast<uint8_t>(t);

        if(r)
        {
            r <<= 8;
            r += id;
        }
        return r;
    }
    
    /**************************************************************************************************************************
     * Returns 16-bit index [ MSB LSB ]
     * MSB : Parameter type
     * LSB : Parameter id/position
     * ***********************************************************************************************************************/
    constexpr uint16_t get_parameter_index(const cell::parameter_type& t, const uint8_t& id)
    {
        uint16_t r = static_cast<uint8_t>(t);

        if(r)
        {
            r <<= 8;
            r += id;
        }
        return r;
    }

    /**************************************************************************************************************************
     * Returns 32-bit hash [ AA BB CC DD  ]
     * AA : Module type
     * BB : Module id/position
     * CC : Parameter type
     * DD : Parameter id/position
     * ***********************************************************************************************************************/
	constexpr uint32_t encode_uid(const uid_t& uid) noexcept
    {
        uint16_t msb = cell::get_module_index(uid.module, uid.module_position);
        uint16_t lsb = cell::get_parameter_index(uid.parameter, uid.parameter_position);
        uint32_t id = msb;
        id <<= 16;
        id += lsb;
        return id;
    }

    uid_t decode_uid(const uint32_t& uid) noexcept
    {
        uint32_t id = uid;
        uid_t _uid;
        _uid.parameter_position = id & 0xFF;
        id >>= 8;
        _uid.parameter = static_cast<parameter_type>(id & 0xFF);
        id >>= 8;
        _uid.module_position = id & 0xFF;
        id >>= 8;
        _uid.module = static_cast<module_type>(id & 0xFF);
        return _uid;
    }
 
	namespace interface
	{

        constexpr uint32_t registry_t::get_hash(const parameter_type& t, const int& p) const
        {
            switch (t) 
            {
                case parameter_type::cc:
                    return hash_table_c[p];
                    break;
                case parameter_type::ic:
                    return hash_table_i[p];
                    break;
                case parameter_type::oc:
                    return hash_table_o[p];
                    break;
                default:
                    break;
            }
            return 0xFFFFFFFF;

        }

		constexpr int registry_t::get_index(const uint32_t& hash) const
		{
            parameter_type t = static_cast<parameter_type>((hash >> 8) & 0xFF);
            switch (t) 
            {
                case parameter_type::cc:
                    for(int i = 0; i < cn; ++i) 
                    {
                       if(hash_table_c.get()[i] == hash) return i;
                    }
                    break;
                case parameter_type::ic:
                    for(int i = 0; i < in; ++i) 
                    {
                       if(hash_table_i.get()[i] == hash) return i;
                    }
                    break;
                case parameter_type::oc:
                    for(int i = 0; i < on; ++i) 
                    {
                       if(hash_table_o.get()[i] == hash) return i;
                    }
                    break;
                default:
                    break;
            }
            return -1;
		}

		void registry_t::fill_lists()
		{
		/*	std::cout<<"Filling lists... "<<settings::module_types<<" module types. \n";

			for(int mt = 0; mt < settings::module_types; ++mt)
			{
				for(int mp = 0; mp < *module[mt].mc; ++mp)
				{
					for(int pp = 0; pp < *module[mt].ic; ++pp)
					{
						uid_t uid { module[mt].mtype, mp, parameter_type::ic, pp };
						hash_table_i[in] = encode_uid(uid);
						//in++;
					}
					for(int pp = 0; pp < *module[mt].oc; ++pp)
					{
						uid_t uid { module[mt].mtype, mp, parameter_type::oc, pp };
						hash_table_i[on] = encode_uid(uid);
					//	on++;
					}
					for(int pp = 0; pp < *module[mt].cc; ++pp)
					{
						uid_t uid { module[mt].mtype, mp, parameter_type::cc, pp };
						hash_table_i[cn] = encode_uid(uid);
					//	cn++;
					}
                   // mn++;
				}
			}
			std::cout<<"-- Lists initialized...\n";*/
		}


	}
}
