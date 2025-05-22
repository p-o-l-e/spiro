#pragma once
#include <memory>
#include <atomic>
#include "constants.hpp"
#include "interface_headers.hpp"

namespace core
{
    class Module
    {
        public:
            const Descriptor* descriptor = &fuse::descriptor;
        private:
            bool standby = false;
        public:

            int position;
            std::unique_ptr<std::atomic<float>*[]> ccv;     // Parameters
            std::unique_ptr<std::atomic<float>*[]> icv;     // Inputs
            std::unique_ptr<std::atomic<float> []> ocv;     // Outputs

            virtual void fuse();
            void init(const uint8_t&, const Descriptor*) noexcept;
            virtual void process() {};

            Module() {};
            virtual ~Module() = default;
            friend class rack_t;
    };
}
