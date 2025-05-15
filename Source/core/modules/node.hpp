/*****************************************************************************************************************************
* Copyright (c) 2022-2025 POLE
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************************************************************/
#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <atomic>
#include <functional>
#include <sys/types.h>
#include <unordered_map>
#include "constants.hpp"

namespace core
{
    enum class module_type    { disconnected, vco, lfo, cso, vca, vcd, vcf, snh, sigma, product, rotor, mixer };
    enum class parameter_type { ctrl = 0xFF, in = 0xEE, out = 0xDD };

    struct uid_t {
        unsigned global_position = 0;
        module_type module = module_type::disconnected;
        unsigned module_position = 0;
        parameter_type parameter;
        unsigned parameter_position = 0;
        uint64_t key;
    };

    inline constexpr uint16_t get_module_index(const module_type& t, const uint8_t& id);
    inline constexpr uint16_t get_parameter_index(const parameter_type& t, const uint8_t& id);
    void  encode_uid(uid_t* uid) noexcept;
    uid_t decode_uid(const uint64_t) noexcept;
    class rack_t;


    class module
    {
        private:
            size_t _cs = 0; // Controls
            size_t _is = 0; // Inputs
            size_t _os = 0; // Outputs
            module_type type = module_type::disconnected;

        public:
            int position;
            std::unique_ptr<std::atomic<float>*[]> ccv;         // Controls
            std::unique_ptr<std::atomic<float>*[]> icv;         // Inputs
            std::unique_ptr<std::atomic<float> []> ocv;         // Outputs
                                    
            virtual void fuse();
            void init(const size_t&, const size_t&, const size_t&, const module_type&, const uint8_t&) noexcept;
            virtual void process() {};

            module() {};
            virtual ~module() = default;
            friend class rack_t;
    };

    class rack_t
    {
        private:
            std::unique_ptr<module*[]>      node;
            std::unique_ptr<uid_t[]>        pin;
            size_t _size = 0;

        public:
            std::atomic<float>* get_output_pin(const uint32_t&) const;
            std::atomic<float>* get_output_pin(const module_type&, const int& module_pos, const int& pin_pos);
            std::atomic<float>** get_input_pin(const uint32_t&) const;
            std::atomic<float>** get_input_pin(const module_type&, const int& module_pos, const int& pin_pos);
            std::atomic<float>** get_ctrl_pin(const uint32_t&) const;
            std::atomic<float>** get_ctrl_pin(const module_type&, const int& module_pos, const int& pin_pos);

            void process(const int& p) { node[p]->process(); }
            void allocate(const int& n) noexcept { node = std::make_unique<module*[]>(n); _size = n; }
            void bind(module* m, const unsigned& pos) { if(pos < _size) node[pos] = m; }
            size_t size() const { return _size; }
            rack_t() {};
           ~rack_t() {};
    };


}
