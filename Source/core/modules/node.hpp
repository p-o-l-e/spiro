#pragma once
#include <atomic>
#include <functional>
#include "descriptor.hxx"

namespace core
{
    template<typename T>
    class Module
    {
        private:
            bool on;
        public:
            const Descriptor* const descriptor;
            const int position;
            std::atomic<T>** ccv;                   // Controls
            std::atomic<T>** icv;                   // Inputs
            std::atomic<T>*  ocv;                   // Outputs
            bool standby() const noexcept { return on; }
            void standby(bool state) noexcept { on = state; }
            virtual void process() noexcept = 0;
            Module(const int, const Descriptor*);
            virtual ~Module();
    };
}

