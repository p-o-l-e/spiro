/*****************************************************************************************************************************
* Quaternion.cpp v1.0.0 30/08/2023
*
* Copyright (c) 2023, Robert Eisele (raw.org)
* Licensed under the MIT license.
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

        constexpr Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
        constexpr Quaternion(const Quaternion &q) : w(q.w), x(q.x), y(q.y), z(q.z) {}
        constexpr Quaternion(float _x, float _y, float _z) : w(0.0f), x(_x), y(_y), z(_z) {}
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
        const float norm() const;
        constexpr float norm_sq() const;
        const Quaternion &normalize();

        constexpr Quaternion conjugate() const;
        constexpr void rotate_vector(float &vx, float &vy, float &vz);

        const void from_euler(const float&, const float&, const float&);
        const void from_axis_angle(const float&, const float&, const float&, const float&);
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

// Calculates the squared length/modulus/magnitude or the norm of a quaternion
constexpr float Quaternion::norm_sq() const
{
	return w * w + x * x + y * y + z * z;
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
	float tx = 2.0f * (y * vz - z * vy);
	float ty = 2.0f * (z * vx - x * vz);
	float tz = 2.0f * (x * vy - y * vx);

	// v + w t + q x t
	vx = vx + w * tx + y * tz - z * ty;
	vy = vy + w * ty + z * tx - x * tz;
	vz = vz + w * tz + x * ty - y * tx;
}

} // namespace core
