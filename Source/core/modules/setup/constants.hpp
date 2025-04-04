#pragma once
#include <atomic>
#include <string>
#include <cmath>

namespace core 
{
    extern std::atomic<float> zero;
    extern std::atomic<float> one;

    constexpr float tao { M_PI * 2.0f };
    constexpr float pi  { M_PI };
    constexpr float chromatic_ratio { powf(2.0f, 1.0f / 12.0f) };

    const std::string lowercase { "abcdefghijklmnopqrstuvwxyz" };
    namespace  settings
    {
        extern int scope_fps;
    }
}
