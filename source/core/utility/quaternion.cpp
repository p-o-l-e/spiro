#include "quaternion.hpp"

namespace core
{
    // Calculates the length/modulus/magnitude or the norm of a quaternion
    const float Quaternion::norm() const
    {
        return sqrtf (w * w + x * x + y * y + z * z);
    }

    // Normalizes the quaternion to have |Q| = 1 as long as the norm is not zero
    const Quaternion& Quaternion::normalize()
    {
        float iLen = 1 / norm();
        w *= iLen;
        x *= iLen;
        y *= iLen;
        z *= iLen;
        return *this;
    }

    // Creates a quaternion by a rotation given by Euler angles (multiplication order from right to left)
    const void Quaternion::from_euler (const float& _x, const float& _y, const float& _z)
    {
        float X = _x * 0.5f;
        float Y = _y * 0.5f;
        float Z = _z * 0.5f;

        float cX = cosf (X);
        float cY = cosf (Y);
        float cZ = cosf (Z);

        float sX = sinf (X);
        float sY = sinf (Y);
        float sZ = sinf (Z);

        // roll around X, pitch around Y, yaw around Z
        // axisAngle([1, 0, 0], φ) * axisAngle([0, 1, 0], θ) * axisAngle([0, 0, 1], ψ)
        w = cX * cY * cZ - sX * sY * sZ;
        x = sX * cY * cZ + sY * sZ * cX;
        y = sY * cX * cZ - sX * sZ * cY;
        z = sX * sY * cZ + sZ * cX * cY;
    }

    // Creates quaternion by a rotation given as axis-angle orientation
    const void Quaternion::from_axis_angle (const float& _x, const float& _y, const float& _z, const float& angle)
    {
        float halfAngle = angle * 0.5;

        float sin_2 = sinf (halfAngle);
        float cos_2 = cosf (halfAngle);

        float sin_norm = sin_2 / sqrtf (_x * _x + _y * _y + _z * _z);

        w = cos_2;
        x = x * sin_norm;
        y = y * sin_norm;
        z = z * sin_norm;
    }

}
