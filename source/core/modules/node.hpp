#pragma once
#include <atomic>
#include <functional>
#include "descriptor.hxx"

namespace core
{
    template<typename T>
    struct Module
    {
            const int position;
            const Descriptor* const descriptor;
            std::atomic<T>** ccv;                   // Controls
            std::atomic<T>** icv;                   // Inputs
            std::atomic<T>*  ocv;                   // Outputs
            virtual void process() noexcept = 0;
            Module(const int, const Descriptor*);
            virtual ~Module();
    };
}

