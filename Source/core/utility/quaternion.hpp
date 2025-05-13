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
* https://github.com/rawify/Quaternion.cpp
******************************************************************************************************************************/
#pragma once
#include <cmath>

namespace core {

class Quaternion
{
    public:
        float w;
        float x;
        float y;
        float z;

        constexpr Quaternion() : w(1), x(0), y(0), z(0) {}
        constexpr Quaternion(const Quaternion &q) : w(q.w), x(q.x), y(q.y), z(q.z) {}
        constexpr Quaternion(float _x, float _y, float _z) : w(0), x(_x), y(_y), z(_z) {}
        constexpr Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

        constexpr Quaternion &operator=(const Quaternion &rhs);
        constexpr Quaternion &operator+=(const Quaternion &q);
        constexpr Quaternion &operator-=(const Quaternion &q);
        constexpr Quaternion &operator*=(float scale);
        constexpr Quaternion &operator*=(const Quaternion &q);

        constexpr Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }
        constexpr Quaternion operator*(const Quaternion &q) const { return Quaternion(*this) *= q; }
        constexpr Quaternion operator*(float scale) const { return Quaternion(w * scale, x * scale, y * scale, z * scale); }
        constexpr Quaternion operator+(const Quaternion &q2) const
        {
            const Quaternion &q1 = *this;
            return Quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
        }
        constexpr Quaternion operator-(const Quaternion &q2) const
        {
            const Quaternion &q1 = *this;
            return Quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
        }

        constexpr float dot(const Quaternion &q) const;
        constexpr float norm() const;
        constexpr float norm_sq() const;
        constexpr Quaternion &normalize();

        constexpr Quaternion conjugate() const;
        constexpr void rotate_vector(float &vx, float &vy, float &vz);

        constexpr void from_euler(const float&, const float&, const float&);
        constexpr void from_axis_angle(const float&, const float&, const float&, const float&);
};

// Assigns a quaternion to the current quaternion
constexpr Quaternion &Quaternion::operator=(const Quaternion &q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

// Adds two quaternions Q1 and Q2
constexpr Quaternion &Quaternion::operator+=(const Quaternion &q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	return *this;
}

// Subtracts a quaternions Q2 from Q1
constexpr Quaternion &Quaternion::operator-=(const Quaternion &q)
{
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return *this;
}

// Scales a quaternion by a scalar
constexpr Quaternion &Quaternion::operator*=(float scale)
{
	w *= scale;
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

// Calculates the Hamilton product of two quaternions
constexpr Quaternion &Quaternion::operator*=(const Quaternion &q)
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
constexpr float Quaternion::dot(const Quaternion &q) const
{
	return w * q.w + x * q.x + y * q.y + z * q.z;
}

// Calculates the length/modulus/magnitude or the norm of a quaternion
constexpr float Quaternion::norm() const
{
	return sqrtf(w * w + x * x + y * y + z * z);
}

// Calculates the squared length/modulus/magnitude or the norm of a quaternion
constexpr float Quaternion::norm_sq() const
{
	return w * w + x * x + y * y + z * z;
}

// Normalizes the quaternion to have |Q| = 1 as long as the norm is not zero
constexpr Quaternion &Quaternion::normalize()
{
	float iLen = 1 / norm();
	w *= iLen;
	x *= iLen;
	y *= iLen;
	z *= iLen;
	return *this;
}

// Calculates the conjugate of a quaternion
constexpr Quaternion Quaternion::conjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

// Rotates a vector according to the current quaternion, assumes |q|=1
// @link https://raw.org/proof/vector-rotation-using-quaternions/
constexpr void Quaternion::rotate_vector(float &vx, float &vy, float &vz)
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
constexpr void Quaternion::from_euler(const float& _x, const float& _y, const float& _z)
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
constexpr void Quaternion::from_axis_angle(const float& _x, const float& _y, const float& _z, const float& angle)
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


} // namespace core
