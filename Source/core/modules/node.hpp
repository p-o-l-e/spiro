#pragma once
#include <memory>
#include <atomic>
#include "interface_headers.hpp"

namespace core
{
    struct Module
    {
        const Descriptor* descriptor = &fuse::descriptor;
        int position;
        std::unique_ptr<std::atomic<float>*[]> ccv;     // Controls
        std::unique_ptr<std::atomic<float>*[]> icv;     // Inputs
        std::unique_ptr<std::atomic<float> []> ocv;     // Outputs

        virtual void fuse();
        virtual void process() noexcept {};
        void init(const uint8_t&, const Descriptor*);
        Module() = default;
        virtual ~Module() = default;
        friend class rack_t;
    };
}
