#pragma once
namespace core {

    template <typename T>
    struct Point2D
    {
        T x;
        T y;

        constexpr Point2D operator+(const Point2D& o) const noexcept
        {
            return { x + o.x, y + o.y };
        }
        
        constexpr Point2D operator-(const Point2D& o) const noexcept
        {
            return { x - o.x, y - o.y };
        }
 
        constexpr Point2D operator-() const noexcept
        {
            return { -x, -y };
        }

        constexpr Point2D operator*(const Point2D& o) const noexcept
        {
            return { x * o.x, y * o.y };
        }

        constexpr Point2D operator/(const Point2D& o) const noexcept
        {
            return { o.x != 0 ? x / o.x : x, o.y != 0 ? y / o.y : y };
        }

        // constexpr Point2D operator*(const T& o) const noexcept
        // {
        //     return { x * o, y * o };
        // }
        //
        friend constexpr Point2D operator*(float o, const Point2D& point) noexcept
        {
            return { static_cast<T>(point.x * o), static_cast<T>(point.y * o) };
        }

        constexpr Point2D operator*(float o) const noexcept
        {
            return { static_cast<T>(x * o), static_cast<T>(y * o) };
        }

        constexpr Point2D operator/(const T& o) const noexcept
        {
            return (o != 0) ? Point2D{x / o, y / o} : *this;
        }

        constexpr Point2D& operator=(const Point2D& o) noexcept
        {
            x = o.x;
            y = o.y;
            return *this;
        }
    };

    template <typename T>
    struct Point3D
    {
        T x;
        T y;
        T z;
    };

    template <typename T>
    struct Rectangle
    {
        T x;
        T y;
        T w;
        T h;
    };

    template <typename T>
    struct RadialSquare
    {
        T xCentre;
        T yCentre;
        T radius;
    };

} // namespace core
