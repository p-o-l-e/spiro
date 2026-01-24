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
*
******************************************************************************************************************************/
#pragma once
#include <iostream>
#include <atomic>

namespace core {

    template <typename T>
    class wavering
    {
        private:
            T* data;
            T* i;
            T* o;
            std::atomic<unsigned long> got = 0;

        public:
            const int segments;
            constexpr void set(const T&) noexcept;
            constexpr T  get() noexcept;
            void advance(int) noexcept;
            unsigned long written() noexcept { return got.load(std::memory_order_relaxed); }
            constexpr T* raw() const noexcept { return data; }
            constexpr wavering(const int& n);
            wavering& operator=(const wavering&);
            wavering(const wavering&);
           ~wavering() { delete[] data; }
    };

    template <typename T>
    constexpr void wavering<T>::set(const T& value) noexcept
    {
        *i = value;
        if(++i >= data + segments)[[unlikely]]
        {
            i = data;
           *i = value;
        }
        got.fetch_add(1, std::memory_order_relaxed);
    }

    template <typename T>
    constexpr T wavering<T>::get() noexcept
    {
        T value = *o;
        if(++o >= data + segments)[[unlikely]] o = data;
        return value;
    }

    template <typename T>
    void wavering<T>::advance(int n) noexcept
    {
        T* p = o + n;

        while (p >= data + segments)
            p -= segments;

        while (p < data)
            p += segments;

        o = p;
    }

    template <typename T>
    constexpr wavering<T>::wavering(const int& n): segments(n) 
    { 
        data = new T[segments]{}; 
        i = data;
        o = data;
    }

    template <typename T>
    wavering<T>::wavering(const wavering& other): segments(other.segments)
    {
        data = new T[segments] {};
        for (int j = 0; j < segments; ++j)
        {
            data[j] = other.data[j];
        }
        i = data;
        o = data;
    }

    template <typename T>
    wavering<T>& wavering<T>::operator=(const wavering& other)
    {
        if (this != &other)
        {
            delete[] data;
            data = new T[other.segments] {};
            for (int j = 0; j < other.segments; ++j)
            {
                data[j] = other.data[j];
            }
            i = data;
            o = data;
        }
        return *this;
    }

}; // namespace core


