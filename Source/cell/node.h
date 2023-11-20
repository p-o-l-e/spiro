#pragma once
#include <memory>
#include <atomic>
#include <functional>

#include "constants.hpp"

namespace cell
{
    enum class module_type { vco, map, vca, vcf, snh };

    class module
    {
        private:
            size_t _cs = 0;
            size_t _is = 0;
            size_t _os = 0;

        public:
            std::unique_ptr<std::atomic<float>*[]> ctrl;    // Parameters
            std::unique_ptr<std::atomic<float>*[]> in;      // Inputs
            std::unique_ptr<std::atomic<float> []> out;     // Outputs
                                                           
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
            std::unique_ptr<module*[]> node;
            size_t _size = 0;

        public:
            void process(const int& p) { node[p]->process(); }
            void allocate(const int& n) noexcept { node = std::make_unique<module*[]>(n); _size = n; }
            void bind(module* m, const unsigned& pos) { if(pos < _size) node[pos] = m; }
            size_t size() const { return _size; }
            rack_t() {};
           ~rack_t() {};
    };
}
