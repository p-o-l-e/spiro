#pragma once
#include <atomic>
#include <functional>
#include "descriptor.hxx"

namespace core
{
    template<typename T>
    struct Module
    {
            int position;
            const Descriptor* const descriptor;
            std::atomic<T>** ccv;                   // Controls (pointees are host-owned parameter atomics)
            T** icv;                                // Inputs (point at other modules' outputs, audio thread only)
            T*  ocv;                                // Outputs (audio thread only)
            virtual void process() noexcept = 0;
            virtual void reset() {}
            Module(int, const Descriptor*);
            virtual ~Module();
    };
}

