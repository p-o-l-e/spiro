/*****************************************************************************************************************************
* Copyright (c) 2022-2025 POLE
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
#include "EnvelopeDisplay.h"
#include "PluginProcessor.h"
#include "core/modules/interface/descriptor.hxx"
#include "core/modules/interface/env_interface.hpp"
#include "descriptor.hxx"
#include "env.hpp"
#include "uid.hpp"
#include <iostream>

EnvelopeDisplay::EnvelopeDisplay(Processor* p, const int ID): processor(p), id(ID), NP(this, this, this, this)
{
    for(int i = 0; i < Stages; ++i)
    {
        addAndMakeVisible(NP[i]);
    }
}

void EnvelopeDisplay::sync()
{
    auto module = processor->spiro.rack.at(core::map::module::env, id);
    for(int i = 0; i < nodes; ++i)
    {
        env.node[i + 1].data[core::breakpoint::Form].store(*module->ccv[core::env::ctl::af + i]);
        env.node[i + 1].data[core::breakpoint::Level].store(*module->ccv[core::env::ctl::aa + i] * scope_bounds.h);
        env.node[i + 1].data[core::breakpoint::Time].store(*module->ccv[core::env::ctl::at + i] * scope_bounds.w);
    }
}

void EnvelopeDisplay::transmit()
{
    static const int offset[core::breakpoint::Count]
    {
        core::env::ctl::af,
        core::env::ctl::aa,
        core::env::ctl::at
    };

    static const int type[core::breakpoint::Count]
    {
        core::breakpoint::Form,
        core::breakpoint::Level,
        core::breakpoint::Time
    };

    for(int j = 0; j < core::breakpoint::Count; ++j)
    {
        for(int i = 0; i < nodes; ++i)
        {
            auto uid = core::uid_t { core::map::module::env, id, core::map::cv::c, offset[j] + i }; 
            auto index = core::grid.getIndex(uid);

            processor->parameters[index] = processor->tree.getParameter(core::grid.name(uid, true));
            float value = env.node[i + 1].data[type[j]];
            if(offset[j] == core::env::ctl::aa)
            {
                value /= scope_bounds.h;
            }
            else if(offset[j] == core::env::ctl::at) 
            {
                value /= scope_bounds.w;
            }
            processor->parameters[index]->beginChangeGesture();
            processor->parameters[index]->setValueNotifyingHost(processor->parameters[index]->convertTo0to1(value));
            processor->parameters[index]->endChangeGesture();
        }
    }
}

EnvelopeDisplay::~EnvelopeDisplay()
{
    transmit();
}

void EnvelopeDisplay::updateNodes()
{  
    env.node[0].data[core::breakpoint::Time].store(0.0f);   
    env.node[0].data[core::breakpoint::Level].store(0.0f);
    for(int i = 0; i < Stages; ++i)
    {
        env.node[i + 1].data[core::breakpoint::Level].store(scope_bounds.h - NP[i].y + gap);
        env.node[i + 1].data[core::breakpoint::Time].store(NP[i].x - gap);
    }
    env.node[5].data[core::breakpoint::Time].store(scope_bounds.w);
    env.node[5].data[core::breakpoint::Level].store(0.0f);
}

void EnvelopeDisplay::load()
{
    sync();
    if(env.node[1].data[core::breakpoint::Time].load() < 1.0f) [[unlikely]]
    {
        setDefaults();
        repaint();
        return;
    }
    for(int i = 0; i < Stages; ++i)
    {
        NP[i].y = area.getHeight() - env.node[i + 1].data[core::breakpoint::Level].load() - gap;
        NP[i].x = env.node[i + 1].data[core::breakpoint::Time].load() + gap;
        NP[i].setCentrePosition(NP[i].x, NP[i].y);
    }
    repaint();
}

void EnvelopeDisplay::paint (juce::Graphics& g)
{
    NP[A].cL = gap + 1;
    NP[A].cR = NP[D].x - gap;
    NP[D].cL = NP[A].x + gap;
    NP[D].cR = NP[S].x - gap;
    NP[S].cL = NP[D].x + gap;
    NP[S].cR = NP[R].x - gap;
    NP[R].cL = NP[S].x + gap;
    NP[R].cR = scope_bounds.w  + gap; 

    updateNodes();
    plot(g, 0.0f);
    g.setColour(colour);
    for(int i = 0; i < Stages; ++i)
    {
        g.drawLine(NP[i].x, NP[i].y, NP[i].x, area.getHeight() - gap, 1.0f);
    }
    g.drawHorizontalLine
    (
        area.getY() + area.getHeight() - gap, 
        area.getX() + gap - curve_width * 0.5f, 
        area.getX() + area.getWidth() - gap - curve_width
    ); 
}

void EnvelopeDisplay::resized()
{
    area = getLocalBounds();
    scope_bounds.x = gap;
    scope_bounds.y = gap;
    scope_bounds.w = area.getWidth() - gap * 2;
    scope_bounds.h = area.getHeight() - gap * 2;

    data = std::make_unique<float[]>(scope_bounds.w);

    setDefaults();
    load();
    updateNodes();
    repaint();
}

void EnvelopeDisplay::plot(juce::Graphics& g, float scale)
{
    float h  = area.getHeight();
    
    env.generate(data.get(), scope_bounds.w);

    float py, cy = 0.0f;
    float px, cx = gap;
    g.setColour (colour.withAlpha(opacity));
    for(int i = 0; i < scope_bounds.w ; ++i)
    {
        py = cy;
        px = cx;
        cx = i + gap;
        cy = data[i];
        g.setColour(colour.withAlpha(opacity));
        g.drawVerticalLine(cx, h - cy - gap, h - gap);   // Flood fill
        g.setColour(colour);
        g.drawLine(px  , h - py - gap, cx  , h - cy - gap, curve_width);
    }
    g.drawLine(cx, h - cy - gap, scope_bounds.w + gap, h - gap, curve_width);
}

void EnvelopeDisplay::mouseDown(const juce::MouseEvent& event)
{
    auto l = [this](const int p) 
    {
        env.node[p].data[core::breakpoint::Form].load() >= 3.0f ? 
        env.node[p].data[core::breakpoint::Form].store(0.0f) :
        env.node[p].data[core::breakpoint::Form].store(env.node[p].data[core::breakpoint::Form].load() + 1.0f);
    };
    if     ((event.x >       0) && (event.x < NP[A].x)) l(1);
    else if((event.x > NP[A].x) && (event.x < NP[D].x)) l(2);
    else if((event.x > NP[D].x) && (event.x < NP[S].x)) l(3);
    else if((event.x > NP[S].x) && (event.x < NP[R].x)) l(4);
    else if((event.x > NP[R].x) && (event.x < area.getWidth())) l(5);
    transmit();
    repaint();
}

void EnvelopeDisplay::setDefaults()
{
    NP[A].setBounds(area.getWidth() / 14, area.getHeight() / 10, diameter, diameter);
    NP[D].setBounds(area.getWidth() /  5, area.getHeight() /  3, diameter, diameter);
    NP[S].setBounds(area.getWidth() /  3, area.getHeight() /  3, diameter, diameter);
    NP[R].setBounds(area.getWidth() /  2, area.getHeight() /  2, diameter, diameter);
}
