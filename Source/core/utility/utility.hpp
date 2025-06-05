/*****************************************************************************************************************************
* Copyright (c) 2022-2025 POLE
*
* Envelope follower & Limiter based on: 
* https://www.musicdsp.org/en/latest/Filters/265-output-limiter-using-envelope-follower-in-c.html
* One Pole Low-Pass filter Based on code of: Joel de Guzman. https://github.com/djowel 
* HP Based on code by Patrice Tarrabia https://www.musicdsp.org/en/latest/Filters/38-lp-and-hp-filter.html
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
******************************************************************************************************************************/
#pragma once
#include "iospecs.hpp"
#include "constants.hpp"
#include "primitives.hpp"

namespace core {

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// One pole LP parameter smooth filter ///////////////////////////////////////////////////////////////////////////
struct OnePole 
{
    private:
        float a;
        float b;
        float z;

    public:
        void reset(const float ms)
        {
            a = expf(- tao / (ms * 0.001f * settings::sample_rate));
            b = 1.0f - a;
            z = 0.0f;
        }
        constexpr float process(const float in) noexcept
        {
            z = (in * b) + (z * a);
            return z;
        }
        OnePole() { reset(10.0f); };
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Allpass filter ////////////////////////////////////////////////////////////////////////////////////////////////
struct AllPass
{
    private:
        float y = 0.0f;
    public:
        float a = 0.5f;
        constexpr float process(const float in) noexcept
        {
            float out = y + a * in;
            y = in - a * out;
            return out;
        }    
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Envelope Follower /////////////////////////////////////////////////////////////////////////////////////////////
struct EnvelopeFollower
{
    float a;
    float r;
    float envelope = 0.0f;

    inline void init(const float, const float, const float);
    inline void process(const float in) noexcept;
};

inline void EnvelopeFollower::init(const float aMs, const float rMs, const float sample_rate)
{
    a = pow( 0.01, 1.0 / ( aMs * sample_rate * 0.001 ) );
    r = pow( 0.01, 1.0 / ( rMs * sample_rate * 0.001 ) );
}

inline void EnvelopeFollower::process(const float in) noexcept
{
    float f = fabs(in);
    if (f > envelope) envelope = a * ( envelope - f ) + f;
    else              envelope = r * ( envelope - f ) + f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Limiter ///////////////////////////////////////////////////////////////////////////////////////////////////////
struct Limiter
{
    EnvelopeFollower e;
    float threshold = 0.01f;
    inline void  init(const float, const float, const float);
    inline float process(const float in) noexcept;
    inline Limiter();
};

inline Limiter::Limiter()
{
    init(1.0f, 10.0f, settings::sample_rate);
}

inline void Limiter::init(const float aMs, const float rMs, const float sample_rate)
{
    e.init(aMs, rMs, sample_rate);
}

inline float Limiter::process(const float in) noexcept
{
    float out = in;
    e.process(out);
    if(e.envelope>threshold)
    {
        out /= expf(e.envelope - threshold);
    }
    return out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Crossfader: f == 1? a = max; f==0? b = max ////////////////////////////////////////////////////////////////////
constexpr float xfade(const float a, const float b, const float f) noexcept
{
    return a * f + b * (1.0f - f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3to2 //////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr Point2D<T> c3c2(const Point3D<T>& in, const float a, const float b) noexcept
{
    auto ac = a > 0.5f ? (1.0f - a) * 2.0f : a * 2.0f;
    auto bc = b > 0.5f ? (1.0f - b) * 2.0f : b * 2.0f;
    return Point2D<T> { xfade(in.x, in.y, ac), xfade(in.y, in.z, bc) };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Departed samples to milliseconds //////////////////////////////////////////////////////////////////////////////
constexpr unsigned DStoMS(const unsigned samples, const unsigned sample_rate) noexcept
{
    return samples * 1000 / sample_rate;
}

constexpr unsigned MStoDS(const unsigned ms, const unsigned sample_rate) noexcept
{
    return ms * sample_rate / 1000;
}

constexpr bool bool_from_range(const float value, const float min = 0.0f, const float max = 1.0f) noexcept
{
    return value > ((max - min) * 0.5f);
}

inline const int rand_in_range(const int l, const int r) noexcept
{ 
    return (rand() % (r - l + 1)) + l;
}

constexpr bool sign(const float data) noexcept
{
    return data > 0.0f ? true : false;
}

constexpr bool sign(const int data) noexcept
{
    return data > 0 ? true : false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DC Block filter ///////////////////////////////////////////////////////////////////////////////////////////////
struct DCBlock 
{
    private:
        float eax = 0.0f, ebx = 0.0f;
    public:
        float f = 0.9995f;
        constexpr float process(const float in) noexcept
        {
            ebx = in - eax + f * ebx;
            eax = in;
            return ebx;
        }
};



} // Namespace core
