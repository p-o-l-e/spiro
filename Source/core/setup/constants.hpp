#pragma once
#include <atomic>
#include <string>
#include <cmath>

namespace core 
{
    extern std::atomic<float> zero;
    extern std::atomic<float> one;
    
    constexpr double pi                 = 3.14159265358979323846;
    constexpr double tao                = 6.28318530717958647692;
    constexpr double chromatic_ratio    = 1.05946309435929526456;

    const std::string lowercase { "abcdefghijklmnopqrstuvwxyz" };
    namespace  settings
    {
        constexpr int scope_fps { 24 };
    }
}
