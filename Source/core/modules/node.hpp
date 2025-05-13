#pragma once
#include <memory>
#include <atomic>
#include "constants.hpp"
#include "interface_headers.hpp"

namespace core
{
    class module_t
    {
        public:
            const Descriptor* descriptor = &interface::fuse::descriptor;
        private:
            bool standby = false;
        public:

            int position;
            std::unique_ptr<std::uint32_t[]>       uid;     // Unique IDs list
            std::unique_ptr<std::atomic<float>*[]> ccv;     // Parameters
            std::unique_ptr<std::atomic<float>*[]> icv;     // Inputs
            std::unique_ptr<std::atomic<float> []> ocv;     // Outputs

            virtual void mute() {};
            virtual void fuse() 
            {
                for(int i = 0; i < *descriptor->cv[map::cv::i]; ++i) icv[i] = &zero;
                for(int i = 0; i < *descriptor->cv[map::cv::c]; ++i) ccv[i] = &zero;
                for(int i = 0; i < *descriptor->cv[map::cv::o]; ++i) ocv[i].store(0.0f);
            };

            void init(const uint8_t&, const Descriptor*) noexcept;
            virtual void process() {};

            module_t() {};
            virtual ~module_t() = default;
            friend class rack_t;
    };
}
