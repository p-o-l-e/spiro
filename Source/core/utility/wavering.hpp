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

namespace core {

    template <typename T>
    class wavering
    {
        private:
            T* data;
            std::atomic<int>  i = 0;
            std::atomic<int>  o = 0;

        public:
            const int segments;
            constexpr void set(const T&) noexcept;
            constexpr T  get() noexcept;
            constexpr T* raw() const noexcept { return data; }
            constexpr wavering(const int& n): segments(n) { data = new T[segments]{}; }
            wavering& operator=(const wavering&) = delete;
            wavering(const wavering&) = delete;
           ~wavering() { delete[] data; }
    };

    template <typename T>
    constexpr void wavering<T>::set(const T& value) noexcept
    {
        data[i] = value;
        if (++i >= segments) i = 0;
    }

    template <typename T>
    constexpr T wavering<T>::get() noexcept
    {
        auto value = data[o];
        if (++o >= segments) o = 0;
        return value;
    }

}; // namespace core


