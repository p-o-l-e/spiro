#include "node.hpp"


namespace core
{
    void module::init(const uint8_t& id, const descriptor_t* d) noexcept 
    {
        desc = d;
        ctrl = std::make_unique<std::atomic<float>*[]>(*desc->cc);
        in   = std::make_unique<std::atomic<float>*[]>(*desc->ic);
        out  = std::make_unique<std::atomic<float> []>(*desc->oc);
        uid  = std::make_unique<std::uint32_t[]>(*desc->ic + *desc->oc + *desc->cc);

        fuse();
        position = id;
    }

}
