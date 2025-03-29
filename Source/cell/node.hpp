#pragma once
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <atomic>
#include <functional>
#include <sys/types.h>
#include <unordered_map>
#include "constants.hpp"
#include "interface.h"
#include "interface/descriptor.hpp"

namespace cell
{
    class rack_t;

    class module
    {
        private:
            const descriptor_t* desc = &interface::fuse::descriptor;

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

            void set_uid(const module_type&, const uint8_t&);
            // uid_t get_uid(const int& pin) const { return uid[pin]; }
            virtual void mute() {};
            virtual void fuse() 
            {
                for(int i = 0; i < *desc->ic; ++i)  in[i] = &zero;
                for(int i = 0; i < *desc->cc; ++i)  ctrl[i] = &zero;
                for(int i = 0; i < *desc->oc; ++i)  out[i].store(0.0f);
            };

            void init(const uint8_t& id, const descriptor_t* d) noexcept 
            {
                desc = d;
                ctrl = std::make_unique<std::atomic<float>*[]>(*desc->cc);
                in   = std::make_unique<std::atomic<float>*[]>(*desc->ic);
                out  = std::make_unique<std::atomic<float> []>(*desc->oc);
                uid  = std::make_unique<std::uint32_t[]>(*desc->ic + *desc->oc + *desc->cc);

                fuse();
                position = id;
                set_uid(desc->mtype, id);
            }
            virtual void process() {};

            module() {};
            virtual ~module();
            friend class rack_t;
    };

    class rack_t
    {
        private:
            std::unique_ptr<module*[]>      node;
            void allocate() noexcept;

        public:
            const interface::registry_t* const registry;
            void connect_pin_i(const uint32_t&, std::atomic<float>**);
            void connect_pin_o(const uint32_t&, std::atomic<float>*);
            void connect_pin_c(const uint32_t&, std::atomic<float>*);

            uint32_t get_uid(const module_type&, const int&, const parameter_type&, const int&);
            module* at(const int& pos) { return node.get()[pos]; }
            void process(const int& p) { node[p]->process(); }
            void bind(module* m, const unsigned& pos) { if(pos < registry->modules()) node[pos] = m; }
            rack_t(const interface::registry_t* const d): registry(d) { allocate(); };
           ~rack_t() {};
    };


}
