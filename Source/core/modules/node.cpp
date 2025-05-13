#include "node.hpp"


namespace core
{
    void module_t::init(const uint8_t& id, const Descriptor* d) noexcept 
    {
        descriptor = d;
        ccv = std::make_unique<std::atomic<float>*[]>(*descriptor->cv[map::cv::c]);
        icv = std::make_unique<std::atomic<float>*[]>(*descriptor->cv[map::cv::i]);
        ocv = std::make_unique<std::atomic<float> []>(*descriptor->cv[map::cv::o]);
        uid = std::make_unique<std::uint32_t[]>(*descriptor->cv[map::cv::i] + *descriptor->cv[map::cv::o] + *descriptor->cv[map::cv::c]);

        fuse();
        position = id;
    }

}
