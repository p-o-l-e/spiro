#pragma once
#include <memory>
#include <atomic>
#include "../setup/constants.hpp"
#include "../interface_headers.hpp"
#include "../interface/descriptor.hxx"
// #include "bus.hpp"

namespace core
{
    class module
    {
        private:
            const descriptor_t* desc = &interface::fuse::descriptor;
            bool standby = false;
        public:
            constexpr int controls() const { return *desc->cc; }
            constexpr int inputs()   const { return *desc->ic; }
            constexpr int outputs()  const { return *desc->oc; }
            constexpr module_type get_type() { return desc->mtype; }
            int position;
            std::unique_ptr<std::uint32_t[]>       uid;     // Unique IDs list
            std::unique_ptr<std::atomic<float>*[]> ctrl;    // Parameters
            std::unique_ptr<std::atomic<float>*[]> in;      // Inputs
            std::unique_ptr<std::atomic<float> []> out;     // Outputs

            virtual void mute() {};
            virtual void fuse() 
            {
                for(int i = 0; i < *desc->ic; ++i)  in[i] = &zero;
                for(int i = 0; i < *desc->cc; ++i)  ctrl[i] = &zero;
                for(int i = 0; i < *desc->oc; ++i)  out[i].store(0.0f);
            };

            void init(const uint8_t&, const descriptor_t*) noexcept;
            virtual void process() {};

            module() {};
            virtual ~module() = default;
            friend class rack_t;
    };



}
