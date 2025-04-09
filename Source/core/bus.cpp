#include "bus.hpp"
#include "uid.hpp"
#include <iostream>

namespace core {
namespace interface { 

    bus_connector::bus_connector(const descriptor_t* d): blueprint(d)
    {
        #ifdef DEBUG 
            std::cout<<"-- Initializing bus...\n";
        #endif
        _c = std::make_unique<std::atomic<float>*[]>(blueprint.cc);
        _i = std::make_unique<std::atomic<float>*[]>(blueprint.ic);
        _o = std::make_unique<std::atomic<float>*[]>(blueprint.oc);

        for(int i = 0; i < blueprint.cc; ++i) _c[i] = &core::zero;
        for(int i = 0; i < blueprint.ic; ++i) _i[i] = &core::zero;
        for(int i = 0; i < blueprint.oc; ++i) _o[i] = &core::zero;
        
        #ifdef DEBUG 
            std::cout<<"---- CC: "<<blueprint.cc<<" IC: "<<blueprint.ic<<" OC: "<<blueprint.oc<<"\n";
            std::cout<<"-- Bus initialized...\n";
        #endif
    }

    std::atomic<float>* bus_connector::pin_c(const uint32_t& o) const 
    { 
        auto i = blueprint.get_index(o);
        if(i >= 0) return _c[i]; 
        return &core::zero;
    }

    std::atomic<float>* bus_connector::pin_i(const uint32_t& o) const 
    { 
        auto i = blueprint.get_index(o);
        if(i >= 0) return _i[i]; 
        return &core::zero;
    }
    
    std::atomic<float>* bus_connector::pin_o(const uint32_t& o) const 
    { 
        auto i = blueprint.get_index(o);
        if(i >= 0) return _o[i]; 
        return &core::zero;
    }

} // namespace interface
} // namespace core


