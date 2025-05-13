#pragma once
namespace core {

    template <typename T>
    struct Point2D
    {
        T x;
        T y;
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
