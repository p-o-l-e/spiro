#include <iostream>
#include <string>
#include <format>
#include "spiro.hpp"

using namespace core;


int main()
{
    Spiro spiro(core::settings::descriptor_map);

    std::cout<<"\nControls [CVC]: \n";
    for(int i = 0; i < spiro.rack.bus.blueprint.mc; ++i)
    {
        std::cout<<std::format("{:2})", i)<<" Prefix:["<<*spiro.rack.bus.blueprint.descriptor[i]->prefix<<"] "; 
        std::cout<<"Postfix: ";
        for(int j = 0; j < *spiro.rack.bus.blueprint.descriptor[i]->cv[map::cv::c]; ++j)
        {
            std::cout<<std::format("{:3}:[{:>7}]",j, spiro.rack.bus.blueprint.descriptor[i]->set[map::cv::c]->postfix);
        }
        std::cout<<"\n";
    }

    std::cout<<"\nInputs [CVI]: \n";
    for(int i = 0; i < spiro.rack.bus.blueprint.mc; ++i)
    {
        std::cout<<std::format("{:2})", i)<<" Prefix:["<<*spiro.rack.bus.blueprint.descriptor[i]->prefix<<"] "; 
        std::cout<<"Postfix: ";
        for(int j = 0; j < *spiro.rack.bus.blueprint.descriptor[i]->cv[map::cv::i]; ++j)
        {
            std::cout<<std::format("{:3}:[{:>7}]",j, spiro.rack.bus.blueprint.descriptor[i]->set[map::cv::i]->postfix);
        }
        std::cout<<"\n";
    }

    std::cout<<"\nOutputs [CVO]: \n";
    for(int i = 0; i < spiro.rack.bus.blueprint.mc; ++i)
    {
        std::cout<<std::format("{:2})", i)<<" Prefix:["<<*spiro.rack.bus.blueprint.descriptor[i]->prefix<<"] "; 
        std::cout<<"Postfix: ";
        for(int j = 0; j < *spiro.rack.bus.blueprint.descriptor[i]->cv[map::cv::o]; ++j)
        {
            std::cout<<std::format("{:3}:[{:>7}]",j, spiro.rack.bus.blueprint.descriptor[i]->set[map::cv::o]->postfix);
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
