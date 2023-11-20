/*****************************************************************************************************************************
* Copyright (c) 2022-2023 POLE
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


#include "oscillator.hpp"

namespace cell 
{
    using namespace interface::vco;
    int oscillator::idc = 0;

    void oscillator::set_delta(const unsigned& voice)
    { 
        int n = note[voice] + 12 * roundf(ctrl[static_cast<int>(ctrl::octave)]->load() * 10.0f);
        freq[voice]  = chromatic[n];
        delta[voice] = chromatic[n] * tao / settings::sample_rate; 
        set_fine(voice);
    }

    void oscillator::set_fine(const unsigned& voice)
    {
        float range   = (freq[voice] * chromatic_ratio - freq[voice] / chromatic_ratio) * tao / settings::sample_rate;
        delta[voice] += (ctrl[static_cast<int>(ctrl::detune)]->load() + in[static_cast<int>(in::detune)]->load() - 0.5f) * range * 2.0f;
    }

    inline float oscillator::tomisawa(const int& voice)
    {
        float oa = cosf(phase[voice] + eax[0][voice]);
        eax[0][voice] = (oa + eax[0][voice]) * 0.5f;

        float pw =  in[static_cast<int>(in::pwm)] == &zero ? 
            (0.5f - ctrl[static_cast<int>(ctrl::pwm)]->load()) * tao * 0.98f - pi :
            (0.5f - ctrl[static_cast<int>(ctrl::pwm)]->load() + in[static_cast<int>(in::pwm)]->load());  

        float ob = cosf(phase[voice] + eax[1][voice] + pw);
        eax[1][voice] = (ob + eax[1][voice]) * 0.5f;
        return oa - ob;
    }

    inline float oscillator::pulse(const int& voice)
    {
        float pw =  in[static_cast<int>(in::pwm)] == &zero ? 
            (0.5f - ctrl[static_cast<int>(ctrl::pwm)]->load()) * 2.0f :
            (0.5f - ctrl[static_cast<int>(ctrl::pwm)]->load() + in[static_cast<int>(in::pwm)]->load()) * 2.0f;    

        return fPulse(phase[voice], pw, 0.0001f);
    }

    inline float oscillator::hexagon(const int& voice)
    {
        float pw = in[static_cast<int>(in::pwm)] == &zero ? 
            (0.5f - ctrl[static_cast<int>(ctrl::pwm)]->load()) * pi :
            (0.5f - ctrl[static_cast<int>(ctrl::pwm)]->load() + in[static_cast<int>(in::pwm)]->load()) * pi;

            float feed = (fTriangle(phase[voice], 0.001f) * fSquare(phase[voice] + pw, 0.001f))/pi + (pi * 0.5f - fabsf(pw)) * 0.25f;
        return feed * (pi - fabsf(pw));
    }


    void oscillator::process()
    {
        float accu = 0.0f;
        
        if(ctrl[static_cast<int>(ctrl::freerun)]->load() > 0.5f)
        {
            set_delta(0);

            float fm = powf(ctrl[static_cast<int>(ctrl::fm)]->load(), 3.0f);

            phase[0] += (delta[0] + in[static_cast<int>(in::fm)]->load() * fm);
            if(phase[0] >= pi) phase[0] -= tao;  

            accu = (this->*form[(int)ctrl[static_cast<int>(ctrl::form)]->load()])(0);


            if(in[static_cast<int>(in::pll)] != &zero)
            {
                float f = powf(ctrl[static_cast<int>(ctrl::pll)]->load(), 3.0f);
                phase[0] += fPLL(accu, in[static_cast<int>(in::pll)]->load()) * f;
            }
            if(in[static_cast<int>(in::am)] != &zero)
            {
                accu = xfade(accu * in[static_cast<int>(in::am)]->load(), accu, ctrl[static_cast<int>(ctrl::am)]->load());
            }
            accu *= ctrl[static_cast<int>(ctrl::amp)]->load();
            out[static_cast<int>(out::main)].store(accu);
        }

        else
        {
            for(int i = 0; i < settings::poly; ++i)
            {
                if(gate[i])
                {
                    // o->set_fine(i);
                    set_delta(i);

                    float fm = powf(ctrl[static_cast<int>(ctrl::fm)]->load(), 3.0f);

                    phase[i] += (delta[i] + in[static_cast<int>(in::fm)]->load() * fm);
                    if(phase[i] >= pi) phase[i] -= tao;  

                    float feed = (this->*form[(int)ctrl[static_cast<int>(ctrl::form)]->load()])(i);

                    if(in[static_cast<int>(in::pll)] != &zero) 
                    {
                        float f = powf(ctrl[static_cast<int>(ctrl::pll)]->load(), 3.0f);
                        phase[0] += fPLL(accu, in[static_cast<int>(in::pll)]->load()) * f;
                   }

                    if(in[static_cast<int>(in::am)] != &zero)
                    {
                        feed = xfade(feed * in[static_cast<int>(in::am)]->load(), feed, ctrl[static_cast<int>(ctrl::am)]->load());
                    }                    
                    feed *= (ctrl[static_cast<int>(ctrl::amp)]->load() * velo[i]);

                    if(env[i].stage == SUS) // Sustained
                    {
                        if(release[i]) feed *= env[i].iterate();
                        else feed *= env[i].out;
                    }
                    else feed *= env[i].iterate();

                    if(env[i].stage == OFF) 
                    {
                        gate[i]    = false;
                        release[i] = false;
                        velo[i]    = 0.0f;
                    }

                    accu += feed;
                            
                }
            }
            out[static_cast<int>(out::main)].store(accu);
        }

    }


    void oscillator::reset()
    {
        init(interface::vco::ctrls, interface::vco::ins, interface::vco::outs);
        for(int i = 0; i < settings::poly; ++i)
        {
            phase[i]    = 0;
            delta[i]    = 0;
            gate[i]     = false;
            release[i]  = false;
            eax[0][i]   = 0.0f;
            eax[1][i]   = 0.0f;
            eax[2][i]   = 0.0f;
            note[i]     = 36;
            env[i].reset();
        }
    }


    oscillator::oscillator()
    {
        id = ++idc;
        reset();
    }

    oscillator::~oscillator() = default;


    std::map<std::string, std::atomic<float>*> vco_create_controls_map(oscillator* o)
    {
        std::map<std::string, std::atomic<float>*> list;
        for(int i = 0; i < interface::vco::ctrl_postfix.size(); ++i)
        {
            interface::vco::ctrl p = static_cast<interface::vco::ctrl>(i);
            std::string id { interface::vco::prefix + lowercase.at(o->id) + interface::vco::ctrl_postfix.at(p) };
            list.insert( { id, o->ctrl[i] });
        }
        return list;
    }

    std::map<std::string, std::atomic<float>*> vco_create_inputs_map(oscillator* o)
    {
        std::map<std::string, std::atomic<float>*> list;
        for(int i = 0; i < interface::vco::in_postfix.size(); ++i)
        {
            interface::vco::in p = static_cast<interface::vco::in>(i);
            std::string id { interface::vco::prefix + lowercase.at(o->id) + interface::vco::in_postfix.at(p) };
            list.insert( { id, o->in[i] });
        }
        return list;
    }

    std::map<std::string, std::atomic<float>*> vco_create_outputs_map(oscillator* o)
    {
        std::map<std::string, std::atomic<float>*> list;
        for(int i = 0; i < interface::vco::out_postfix.size(); ++i)
        {
            interface::vco::out p = static_cast<interface::vco::out>(i);
            std::string id { interface::vco::prefix + lowercase.at(o->id) + interface::vco::out_postfix.at(p) };
            list.insert( { id, &o->out[i] });
        }
        return list;
    }

}; // Namespace
