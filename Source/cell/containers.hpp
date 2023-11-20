/*****************************************************************************************************************************
* Copyright (c) 2022-2023 POLE
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
#include <cmath>
#include <string>

namespace cell {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3D Point ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct lcr
{
    T l;
    T c;
    T r;
};

template <typename T>
constexpr lcr<T> RotateX(const lcr<T>& o, const T& theta) noexcept
{
    return lcr<T> { o.l, o.c * cosf(theta) - o.r * sinf(theta), o.c * sinf(theta) + o.r * cosf(theta)};
}

template <typename T>
constexpr lcr<T> RotateY(const lcr<T>& o, const T& theta) noexcept
{
    return lcr<T> { o.l * cosf(theta) + o.r * sinf(theta), o.c, o.r * cosf(theta) - o.l * sinf(theta)};
}

template <typename T>
constexpr lcr<T> RotateZ(const lcr<T>& o, const T& theta) noexcept
{
    return lcr<T> { o.l * cosf(theta) - o.c * sinf(theta), o.l * sinf(theta) + o.c * cosf(theta), o.r};
}

template <typename T>
constexpr lcr<T> Rotate(const lcr<T>& o, const T& x, const T& y, const T& z) noexcept
{
    return RotateZ(RotateY(RotateX(o, x) , y), z);
}

template<typename T> 
constexpr lcr<T> sphericalToCartesian(const T& theta, const T& phi) noexcept
{ 
    return lcr<T> { cosf(phi) * sinf(theta), sinf(phi) * sinf(theta), cosf(theta) }; 
}; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2D Point ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct l_r
{
    T l;
    T r;
};

template <typename T>
struct point
{
    T x;
    T y;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Square /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct square
{
    T xCentre;
    T yCentre;
    T radius;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rectangle //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct rectangle
{
    T x;
    T y;
    T w;
    T h;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Frame //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class frame
{
    private:
        T* data;
    public:   
        const unsigned width;
        const unsigned height;             
        constexpr void set(const unsigned&, const unsigned&, const T&) noexcept;
        constexpr T    get(const unsigned&, const unsigned&) const noexcept;
        constexpr T*   ref(const unsigned&, const unsigned&) const noexcept;
        constexpr T*   ref() const noexcept { return data; }
        constexpr void clr(const T&) noexcept;
        constexpr frame(const unsigned&, const unsigned&) noexcept;
       ~frame();
};


template <typename T>
constexpr void frame<T>::set(const unsigned& x, const unsigned& y, const T& value) noexcept
{
    if((x < width) && (y < height))
    data[x + y * width] = value;
}

template <typename T>
constexpr T frame<T>::get(const unsigned& x, const unsigned& y) const noexcept
{
    if((x < width) && (y < height)) return data[x + y * width];
    return data[0];
}

template <typename T>
constexpr T* frame<T>::ref(const unsigned& x, const unsigned& y) const noexcept
{
    if((x < width) && (y < height)) return &data[x + y * width];
    return &data[0];
}

template <typename T>
constexpr void frame<T>::clr(const T& value) noexcept
{
    for(unsigned i = 0; i < (height * width); ++i) data[i] = value;
}

template <typename T>
constexpr frame<T>::frame(const unsigned& x, const unsigned& y) noexcept: width(x), height(y)
{
    data = new T[height * width];
}

template <typename T>
inline frame<T>::~frame()
{
    delete[] data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Wavering ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class wavering
{
    private:
        T* data;
        int  i = 0; // Write Pointer
        int  o = 0; // Read Pointer
    public:
        const int segments = 1024;
        constexpr void set(const T&) noexcept;
        constexpr void set(const unsigned&, const T&) noexcept;
        constexpr T get() noexcept;
        constexpr T get(const int&) noexcept;
        constexpr T* raw() const noexcept { return data; }
        constexpr wavering() noexcept { data = new T[segments]; }
        constexpr wavering(const int& n): segments(n) { data = new T[segments]; }
       ~wavering() { delete[] data; }
};

template <typename T>
constexpr void wavering<T>::set(const T& value) noexcept
{
    i++;
    if (i >= segments) i = 0;
    data[i] = value;
}

template <typename T>
constexpr void wavering<T>::set(const unsigned& pos, const T& value) noexcept
{
    data[pos % segments] = value;
}

template <typename T>
constexpr T wavering<T>::get() noexcept
{
    o++;
    if (o >= segments) o = 0;
    return data[o];
}

template <typename T>
constexpr T wavering<T>::get(const int& offset) noexcept
{
    o += offset;
    if(o < 0) o = segments - o;
    else o %= segments;
    return data[o];
}


}; // namespace cell


