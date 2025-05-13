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
    class Canvas
    {
        private:
            T* data;
        public:   
            const unsigned width;
            const unsigned height;             
            constexpr void set(const unsigned&, const unsigned&, const T&) noexcept;
            constexpr T    get(const unsigned&, const unsigned&) const noexcept;
            constexpr T*   raw() const noexcept { return data; }
            constexpr void clr(const T&) noexcept;
            constexpr Canvas(const unsigned&, const unsigned&) noexcept;
           ~Canvas();
    };


template <typename T>
constexpr void Canvas<T>::set(const unsigned& x, const unsigned& y, const T& value) noexcept
{
    if((x < width) && (y < height))
    data[x + y * width] = value;
}

template <typename T>
constexpr T Canvas<T>::get(const unsigned& x, const unsigned& y) const noexcept
{
    if((x < width) && (y < height)) return data[x + y * width];
    return data[0];
}

template <typename T>
constexpr void Canvas<T>::clr(const T& value) noexcept
{
    for(unsigned i = 0; i < (height * width); ++i) data[i] = value;
}

template <typename T>
constexpr Canvas<T>::Canvas(const unsigned& x, const unsigned& y) noexcept: width(x), height(y)
{
    data = new T[height * width];
}

template <typename T>
inline Canvas<T>::~Canvas()
{
    delete[] data;
}
}; // namespace core


