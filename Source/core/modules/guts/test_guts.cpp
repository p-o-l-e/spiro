#include <iostream>
#include <string>
#include "uid.hpp"
#include "../setup/blueprint.hpp"
#include "bus.hpp"

using namespace core;



int main()
{
    std::cout<<"\nGuts Test\n\n";
    std::cout<<"From values:\n";
    core::uid_t uid { module_type::com, parameter_type::cc, 2, 4 };
    
    std::cout<<"Hash               : "<<std::hex<<uid.hash<<"\n";
    std::cout<<"Module type        : "<<static_cast<int>(uid.module)<<"\n";
    std::cout<<"Parameter type     : "<<static_cast<int>(uid.parameter)<<"\n";
    std::cout<<"Module position    : "<<static_cast<int>(uid.module_position)<<"\n";
    std::cout<<"Parameter position : "<<static_cast<int>(uid.parameter_position)<<"\n";

    std::cout<<"\nGuts Test\n\n";
    std::cout<<"From hash:\n";

    core::uid_t uid2 { uid.hash };
    
    std::cout<<"Hash               : "<<std::hex<<uid2.hash<<"\n";
    std::cout<<"Module type        : "<<static_cast<int>(uid2.module)<<"\n";
    std::cout<<"Parameter type     : "<<static_cast<int>(uid2.parameter)<<"\n";
    std::cout<<"Module position    : "<<static_cast<int>(uid2.module_position)<<"\n";
    std::cout<<"Parameter position : "<<static_cast<int>(uid2.parameter_position)<<"\n";

    uint32_t d { 0xAABBCCDD }; 
    for(int i = 0; i < 4; ++i)
    {
        std::cout<<"Byte "<<i<<" : "<<std::hex<<(int)extract_byte(d, i)<<"\n";
    }



    const core::interface::blueprint_t bp(core::settings::descriptor_map);

    std::cout<<"Inputs   : "<<bp.ic<<"\n";
    std::cout<<"Outputs  : "<<bp.oc<<"\n";
    std::cout<<"Controls : "<<bp.cc<<"\n";
    std::cout<<"Modules  : "<<bp.mc<<"\n";

    for(int i = 0; i < core::settings::n_modules(); ++i)
    {
        
        std::cout<<i<<") Prefix:[ "<<*bp.descriptor[i].prefix<<" ]\n";
        std::cout<<"\t";
        for(int j = 0; j < *bp.descriptor[i].cc; ++j)
        {
            std::cout<<j<<":[ "<<bp.descriptor[i].set_c[j].postfix<<" ] ";
        }
        std::cout<<"\n";
    }

    for(int i = 0; i < core::settings::n_modules(); ++i)
    {
        std::cout<<"Relative : "<<bp.relative[i]<<"\n";
    }

    const core::interface::module_map mm(&bp);
    std::cout<<"\n";
    for(int i = 0; i < mm.blueprint->cc; ++i) std::cout<<mm.hash_table_c[i]<<" : ";
    std::cout<<"\n";
    for(int i = 0; i < mm.blueprint->ic; ++i) std::cout<<mm.hash_table_i[i]<<" : ";
    std::cout<<"\n";
    for(int i = 0; i < mm.blueprint->oc; ++i) std::cout<<mm.hash_table_o[i]<<" : ";
    std::cout<<"\n";

    return 0;
}