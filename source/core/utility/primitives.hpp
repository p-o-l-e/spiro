#pragma once
namespace core {

    template <typename T>
    struct Point2D
    {
        T x = T {};
        T y = T {}; 

        constexpr Point2D() = default;
        constexpr Point2D(T x_, T y_) : x(x_), y(y_) {}
        constexpr Point2D(T o) : x(o), y(o) {}

        constexpr Point2D& operator+=(const Point2D& rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        constexpr Point2D& operator-=(const Point2D& rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        constexpr Point2D& operator*=(T s) noexcept
        {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr Point2D& operator/=(T s) noexcept
        {
            x /= s;
            y /= s;
            return *this;
        }

        constexpr Point2D operator+(const Point2D& rhs) const noexcept
        {
            Point2D tmp = *this;
            tmp += rhs;
            return tmp;
        }

        constexpr Point2D operator-(const Point2D& rhs) const noexcept
        {
            Point2D tmp = *this;
            tmp -= rhs;
            return tmp;
        }

        constexpr Point2D operator*(T s) const noexcept
        {
            Point2D tmp = *this;
            tmp *= s;
            return tmp;
        }

        constexpr Point2D operator/(T s) const noexcept
        {
            Point2D tmp = *this;
            tmp /= s;
            return tmp;
        }

        constexpr Point2D operator/(Point2D& rhs) const noexcept
        {
            Point2D tmp = *this;
            tmp.x /= rhs.x;
            tmp.y /= rhs.y;
            return tmp;
        }
    };

    template <typename T>
    struct Point3D
    {
        T x = T {};
        T y = T {};
        T z = T {};
    };

    template <typename T>
    struct Rectangle
    {
        T x = T {};
        T y = T {};
        T w = T {};
        T h = T {};
    };
    
    template <typename T>
    struct RectangleLTRB
    {
        T l = T {};
        T t = T {};
        T r = T {};
        T b = T {};
    };

    template <typename T>
    struct SquareXYR
    {
        T x = T {};
        T y = T {};
        T r = T {};
    };
} // namespace core
