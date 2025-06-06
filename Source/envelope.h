#pragma once
#include <atomic>
#include "env.hpp"


namespace core {

struct breakpoint
{
    enum { Time, Amplitude, Form, Count };
    std::atomic<float> data[Count] {};
};

class Envelope
{
    private:
        float theta = 0.0f;                 // Amplitude difference
        unsigned delta = 0;
        float prior = 0.0f;
    public:
        breakpoint  node[SEGMENTS];
        unsigned    time[SEGMENTS];
        float      value[SEGMENTS];
        float      curve[SEGMENTS];
        uint  departed;                     // Current sample
        int   stage = 0;                    // Current stage
        void  start();          
        void  next_stage();
        void  reset();
        float iterate();
        bool  regenerate = false;
        void  generate(float*, const int);        // Compute Envelope to given array
        Envelope();
       ~Envelope() = default;
};

inline void Envelope::start()
{
    stage = 1;
    departed = 0;
    for(int i = 0; i < SEGMENTS; ++i)
    {
        value[i] = node[i].data[breakpoint::Amplitude].load();
        time[i]  = node[i].data[breakpoint::Time].load();
        curve[i] = node[i].data[breakpoint::Form].load();
    }
    theta = value[stage] - value[stage - 1];
    delta = time[stage] - time[stage - 1];
}

inline void Envelope::reset()
{
    stage    = 0;
    departed = 0;
    for(int i = 0; i < SEGMENTS; ++i)
    {
        time[i]  = 0;
        value[i] = 0.0f;
        curve[i] = 0.0f;
    }
}

inline void Envelope::next_stage()
{
    stage++;
    departed = 0;
    if  (stage >= SEGMENTS)  stage = OFF;
    else
    {
        theta = value[stage] - value[stage - 1];
        delta = time[stage] - time[stage - 1];
    }
}

inline float Envelope::iterate()
{
    if(stage > 0)
    {
        prior = ease[(int)curve[stage]](float(departed), value[stage - 1], theta, float(delta));
        departed++;
        if (departed >= delta) next_stage();
        return prior;
    }
    return 0.0f;
}

inline void Envelope::generate(float* data, const int width)
{
    reset();
    start();
    for(int i = 0; i < width; ++i) data[i] = iterate();
    regenerate = false;
}

inline Envelope::Envelope()
{
    reset();
}

}
