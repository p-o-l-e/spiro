#include <iostream>
#include <string>
#include <format>
#include "uid.hpp"
#include "bus.hpp"
#include "rack.hpp"
#include "spiro.hpp"

using namespace core;
#define DEBUG


int main()
{
    spiro_t spiro(core::settings::descriptor_map);

    std::cout<<"\nControls [CVC]: \n";
    for(int i = 0; i < spiro.rack.bus.blueprint.mc; ++i)
    {
        std::cout<<std::format("{:2})", i)<<" Prefix:["<<*spiro.rack.bus.blueprint.descriptor[i].data()->prefix<<"] "; 
        std::cout<<"Postfix: ";
        for(int j = 0; j < *spiro.rack.bus.blueprint.descriptor[i].data()->cc; ++j)
        {
            std::cout<<std::format("{:3}:[{:>7}]",j, spiro.rack.bus.blueprint.descriptor[i].data()->set_c[j].postfix);
        }
        std::cout<<"\n";
    }

    std::cout<<"\nInputs [CVI]: \n";
    for(int i = 0; i < spiro.rack.bus.blueprint.mc; ++i)
    {
        std::cout<<std::format("{:2})", i)<<" Prefix:["<<*spiro.rack.bus.blueprint.descriptor[i].data()->prefix<<"] "; 
        std::cout<<"Postfix: ";
        for(int j = 0; j < *spiro.rack.bus.blueprint.descriptor[i].data()->ic; ++j)
        {
            std::cout<<std::format("{:3}:[{:>7}]",j, spiro.rack.bus.blueprint.descriptor[i].data()->set_i[j].postfix);
        }
        std::cout<<"\n";
    }

    std::cout<<"\nOutputs [CVO]: \n";
    for(int i = 0; i < spiro.rack.bus.blueprint.mc; ++i)
    {
        std::cout<<std::format("{:2})", i)<<" Prefix:["<<*spiro.rack.bus.blueprint.descriptor[i].data()->prefix<<"] "; 
        std::cout<<"Postfix: ";
        for(int j = 0; j < *spiro.rack.bus.blueprint.descriptor[i].data()->oc; ++j)
        {
            std::cout<<std::format("{:3}:[{:>7}]",j, spiro.rack.bus.blueprint.descriptor[i].data()->set_o[j].postfix);
        }
        std::cout<<"\n";
    }

    std::cout<<"\nRelatives : ";
    for(int i = 0; i < spiro.rack.bus.blueprint.mc; ++i)
    {
        std::cout<<spiro.rack.bus.blueprint.relative[i]<<" ";
    }
    std::cout<<"\n";

    return 0;
}