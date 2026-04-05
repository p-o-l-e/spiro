#pragma once
#include <atomic>
#include <string>
#include <cmath>

namespace core 
{

#define CORE_FLOAT
#ifdef CORE_FLOAT

    extern std::atomic<float> zero;
    extern std::atomic<float> one;
    
    constexpr float pi                 = 3.14159265358979323846f;
    constexpr float tao                = 6.28318530717958647692f;
    constexpr float chromatic_ratio    = 1.05946309435929526456f;

#elifdef CORE_DOUBLE

    extern std::atomic<double> zero;
    extern std::atomic<double> one;
    
    constexpr double pi                 = 3.14159265358979323846;
    constexpr double tao                = 6.28318530717958647692;
    constexpr double chromatic_ratio    = 1.05946309435929526456;

#endif

    const std::string lowercase { "abcdefghijklmnopqrstuvwxyz" };
    namespace  settings
    {
        constexpr int scope_fps { 24 };
    }
}
