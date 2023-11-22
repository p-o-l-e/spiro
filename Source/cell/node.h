#pragma once
#include <cstdint>
#include <memory>
#include <atomic>
#include <functional>

#include "constants.hpp"

namespace cell
{
    enum class module_type    { disconnected, vco, map, vca, vcf, snh };
    enum class parameter_type { ctrl = 0xFF, in = 0xEE, out = 0xDD };

    class module
    {
        private:
            size_t _cs = 0;
            size_t _is = 0;
            size_t _os = 0;
            module_type type = module_type::disconnected;

        public:
            std::unique_ptr<std::uint32_t[]>       uid;     // Unique ID
            std::unique_ptr<std::atomic<float>*[]> ctrl;    // Parameters
            std::unique_ptr<std::atomic<float>*[]> in;      // Inputs
            std::unique_ptr<std::atomic<float> []> out;     // Outputs
                                                        
            void set_uid(const module_type&, const uint8_t&);

                                                           
            virtual void fuse() 
            {
                for(int i = 0; i < _is; ++i)  in[i] = &zero;
                for(int i = 0; i < _cs; ++i)  ctrl[i] = &zero;
                for(int i = 0; i < _os; ++i)  out[i].store(0.0f);
            };

            void init(const size_t& ctrls, const size_t& ins, const size_t& outs) noexcept 
            {
                ctrl = std::make_unique<std::atomic<float>*[]>(ctrls);
                in   = std::make_unique<std::atomic<float>*[]>(ins);
                out  = std::make_unique<std::atomic<float> []>(outs);
                uid  = std::make_unique<std::uint32_t[]>(ctrls + ins + outs);
                _cs = ctrls;
                _is = ins;
                _os = outs;
                fuse();
            }
            virtual void process() {};

            module() {};
            virtual ~module();
    };

    class rack_t
    {
        private:
            std::unique_ptr<module*[]>      node;
            size_t _size = 0;

        public:
            void process(const int& p) { node[p]->process(); }
            void allocate(const int& n) noexcept { node = std::make_unique<module*[]>(n); _size = n; }
            void bind(module* m, const unsigned& pos) { if(pos < _size) node[pos] = m; }
            size_t size() const { return _size; }
            rack_t() {};
           ~rack_t() {};
    };

    /**************************************************************************************************************************
     * Returns 16-bit index [ MSB LSB ]
     * MSB : Module type
     * LSB : Module id
     * ***********************************************************************************************************************/
    inline uint16_t get_module_index(module_type t, uint8_t id)
    {
        uint16_t r = static_cast<uint8_t>(t);

        if(r)
        {
            r <<= 8;
            r += id;
        }
        return r;
    }
    
    /**************************************************************************************************************************
     * Returns 16-bit index [ MSB LSB ]
     * MSB : Parameter type
     * LSB : Parameter id
     * ***********************************************************************************************************************/
    inline uint16_t get_parameter_index(parameter_type t, uint8_t id)
    {
        uint16_t r = static_cast<uint8_t>(t);

        if(r)
        {
            r <<= 8;
            r += id;
        }
        return r;
    }

}
