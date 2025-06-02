#include "node.hpp"
#include "constants.hpp"

namespace core
{
    void Module::fuse() 
    {
        for(int i = 0; i < *descriptor->cv[map::cv::i]; ++i) icv[i] = &zero;
        for(int i = 0; i < *descriptor->cv[map::cv::c]; ++i) ccv[i] = &zero;
        for(int i = 0; i < *descriptor->cv[map::cv::o]; ++i) ocv[i].store(0.0f);
    };

    void Module::init(const uint8_t& id, const Descriptor* d)
    {
        descriptor = d;
        ccv = std::make_unique<std::atomic<float>*[]>(*descriptor->cv[map::cv::c]);
        icv = std::make_unique<std::atomic<float>*[]>(*descriptor->cv[map::cv::i]);
        ocv = std::make_unique<std::atomic<float> []>(*descriptor->cv[map::cv::o]);

        fuse();
        position = id;
    }
}
