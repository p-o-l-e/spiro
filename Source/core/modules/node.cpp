#include "node.hpp"


namespace core
{
    void module_t::init(const uint8_t& id, const descriptor_t* d) noexcept 
    {
        descriptor = d;
        ccv = std::make_unique<std::atomic<float>*[]>(*descriptor->cc);
        icv = std::make_unique<std::atomic<float>*[]>(*descriptor->ic);
        ocv = std::make_unique<std::atomic<float> []>(*descriptor->oc);
        uid = std::make_unique<std::uint32_t[]>(*descriptor->ic + *descriptor->oc + *descriptor->cc);

        fuse();
        position = id;
    }

}
