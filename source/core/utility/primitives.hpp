#pragma once
namespace core {

    template <typename T>
    struct Point2D
    {
        T x = T {};
        T y = T {};
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
