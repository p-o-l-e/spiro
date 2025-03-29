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
*
* https://github.com/rawify/Quaternion.cpp
******************************************************************************************************************************/


#pragma once
#include <cmath>
#include <string>

namespace cell {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Point //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct point2d
{
    T x;
    T y;
};

template <typename T>
struct point3d
{
    T x;
    T y;
    T z;
};

class quaternion
{
    public:
        float w;
        float x;
        float y;
        float z;

        constexpr quaternion() : w(1), x(0), y(0), z(0) {}
        constexpr quaternion(const quaternion &q) : w(q.w), x(q.x), y(q.y), z(q.z) {}
        constexpr quaternion(float _x, float _y, float _z) : w(0), x(_x), y(_y), z(_z) {}
        constexpr quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

        constexpr quaternion &operator=(const quaternion &rhs);
        constexpr quaternion &operator+=(const quaternion &q);
        constexpr quaternion &operator-=(const quaternion &q);
        constexpr quaternion &operator*=(float scale);
        constexpr quaternion &operator*=(const quaternion &q);

        constexpr quaternion operator-() const { return quaternion(-w, -x, -y, -z); }
        constexpr quaternion operator*(const quaternion &q) const { return quaternion(*this) *= q; }
        constexpr quaternion operator*(float scale) const { return quaternion(w * scale, x * scale, y * scale, z * scale); }
        constexpr quaternion operator+(const quaternion &q2) const
        {
            const quaternion &q1 = *this;
            return quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
        }
        constexpr quaternion operator-(const quaternion &q2) const
        {
            const quaternion &q1 = *this;
            return quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
        }

        constexpr float dot(const quaternion &q) const;
        constexpr float norm() const;
        constexpr float norm_sq() const;
        constexpr quaternion &normalize();

        constexpr quaternion conjugate() const;
        constexpr void rotate_vector(float &vx, float &vy, float &vz);

        constexpr void from_euler(const float&, const float&, const float&);
        constexpr void from_axis_angle(const float&, const float&, const float&, const float&);
};


// Assigns a quaternion to the current quaternion
constexpr quaternion &quaternion::operator=(const quaternion &q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}


// Adds two quaternions Q1 and Q2
constexpr quaternion &quaternion::operator+=(const quaternion &q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	return *this;
}

// Subtracts a quaternions Q2 from Q1
constexpr quaternion &quaternion::operator-=(const quaternion &q)
{
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return *this;
}

// Scales a quaternion by a scalar
constexpr quaternion &quaternion::operator*=(float scale)
{
	w *= scale;
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

// Calculates the Hamilton product of two quaternions
constexpr quaternion &quaternion::operator*=(const quaternion &q)
{
	float w1 = w;
	float x1 = x;
	float y1 = y;
	float z1 = z;

	float w2 = q.w;
	float x2 = q.x;
	float y2 = q.y;
	float z2 = q.z;

	w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
	x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
	y = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
	z = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
	return *this;
}

// Calculates the dot product of two quaternions
constexpr float quaternion::dot(const quaternion &q) const
{
	return w * q.w + x * q.x + y * q.y + z * q.z;
}

// Calculates the length/modulus/magnitude or the norm of a quaternion
constexpr float quaternion::norm() const
{
	return sqrtf(w * w + x * x + y * y + z * z);
}

// Calculates the squared length/modulus/magnitude or the norm of a quaternion
constexpr float quaternion::norm_sq() const
{
	return w * w + x * x + y * y + z * z;
}

// Normalizes the quaternion to have |Q| = 1 as long as the norm is not zero
constexpr quaternion &quaternion::normalize()
{
	float iLen = 1 / norm();
	w *= iLen;
	x *= iLen;
	y *= iLen;
	z *= iLen;
	return *this;
}

// Calculates the conjugate of a quaternion
constexpr quaternion quaternion::conjugate() const
{
	return quaternion(w, -x, -y, -z);
}

// Rotates a vector according to the current quaternion, assumes |q|=1
// @link https://raw.org/proof/vector-rotation-using-quaternions/
constexpr void quaternion::rotate_vector(float &vx, float &vy, float &vz)
{
	// t = 2q x v
	float tx = 2. * (y * vz - z * vy);
	float ty = 2. * (z * vx - x * vz);
	float tz = 2. * (x * vy - y * vx);

	// v + w t + q x t
	vx = vx + w * tx + y * tz - z * ty;
	vy = vy + w * ty + z * tx - x * tz;
	vz = vz + w * tz + x * ty - y * tx;
}

// Creates a quaternion by a rotation given by Euler angles (multiplication order from right to left)
constexpr void quaternion::from_euler(const float& _x, const float& _y, const float& _z)
{
	float X = _x * 0.5f;
	float Y = _y * 0.5f;
	float Z = _z * 0.5f;

	float cX = cosf(X);
	float cY = cosf(Y);
	float cZ = cosf(Z);

	float sX = sinf(X);
	float sY = sinf(Y);
	float sZ = sinf(Z);

    // roll around X, pitch around Y, yaw around Z
	// axisAngle([1, 0, 0], φ) * axisAngle([0, 1, 0], θ) * axisAngle([0, 0, 1], ψ)
	w =	cX * cY * cZ - sX * sY * sZ;
	x =	sX * cY * cZ + sY * sZ * cX;
	y =	sY * cX * cZ - sX * sZ * cY;
	z =	sX * sY * cZ + sZ * cX * cY;
}

// Creates quaternion by a rotation given as axis-angle orientation
constexpr void quaternion::from_axis_angle(const float& _x, const float& _y, const float& _z, const float& angle)
{
	float halfAngle = angle * 0.5;

	float sin_2 = sinf(halfAngle);
	float cos_2 = cosf(halfAngle);

	float sin_norm = sin_2 / sqrtf(_x * _x + _y * _y + _z * _z);

	w = cos_2;
	x = x * sin_norm;
	y = y * sin_norm;
	z = z * sin_norm;
}



template <typename T>
constexpr point3d<T> RotateX(const point3d<T>& o, const T& theta) noexcept
{
    return point3d<T> { o.l, o.c * cosf(theta) - o.r * sinf(theta), o.c * sinf(theta) + o.r * cosf(theta)};
}

template <typename T>
constexpr point3d<T> RotateY(const point3d<T>& o, const T& theta) noexcept
{
    return point3d<T> { o.l * cosf(theta) + o.r * sinf(theta), o.c, o.r * cosf(theta) - o.l * sinf(theta)};
}

template <typename T>
constexpr point3d<T> RotateZ(const point3d<T>& o, const T& theta) noexcept
{
    return point3d<T> { o.l * cosf(theta) - o.c * sinf(theta), o.l * sinf(theta) + o.c * cosf(theta), o.r};
}

template <typename T>
constexpr point3d<T> Rotate(const point3d<T>& o, const T& x, const T& y, const T& z) noexcept
{
    return RotateZ(RotateY(RotateX(o, x) , y), z);
}

template<typename T> 
constexpr point3d<T> sphericalToCartesian(const T& theta, const T& phi) noexcept
{ 
    return point3d<T> { cosf(phi) * sinf(theta), sinf(phi) * sinf(theta), cosf(theta) }; 
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
// Square /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct square
{
    T xCentre;
    T yCentre;
    T radius;
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


