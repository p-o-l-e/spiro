/*****************************************************************************************************************************
* Copyright (c) 2022-2026 POLE
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
*
******************************************************************************************************************************/
#pragma once
#include "env_interface.hpp"
#include <atomic>
#include <cstddef>
#include <iostream>

namespace core {

    template <typename T>
    class wavering
    {
        private:
            const std::size_t size;
            const std::size_t mask;
            std::size_t r = 0;
            std::size_t w = 0;
            std::atomic<unsigned long> counter = 0;
            T* data = nullptr;

        public:
            void set(const T& value) noexcept
            {
                data[w] = value;
                w = (w + 1) & mask;
            }

            void add(unsigned long n) noexcept
            {
                counter.fetch_add(n, std::memory_order_relaxed);
            }

            T get() noexcept
            {
                T value = data[r];
                r = (r + 1) & mask;
                return value;
            }

            T get(size_t jump) noexcept
            {
                T value = data[r];
                r = (r + jump) & mask;
                return value;
            }

            void advance(std::size_t n) noexcept
            {
                r = (r + n) & mask;
            }

            T* raw() const noexcept { return data; }
            unsigned long count() noexcept { return counter.load(std::memory_order_relaxed); }

            explicit wavering(std::size_t n): size(std::size_t{ 1 } << n), mask(size - 1)
            {
                data = new T[size]{};
            }

            ~wavering() { delete[] data; }
    };


} // namespace core


