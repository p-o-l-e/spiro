/*****************************************************************************************************************************
* Spiro
* Copyright (C) 2022-2023 POLE
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************************************************************/

#include "EnvelopeDisplay.h"


EnvelopeDisplay::EnvelopeDisplay (): npa(this), npd(this), nps(this), npr(this)
{
    for(int i = 0; i < SEGMENTS; ++i)
    {
        envd.node[i].time  = &node[i].time;
        envd.node[i].value = &node[i].value;
        envd.node[i].curve = &node[i].curve;
    }
    envd.time_scale = &cell::one;
    envd.time_multiplier = 1.0f;
    envd.value_scale = 1.0f;

    addAndMakeVisible(npa);
    addAndMakeVisible(npd);
    addAndMakeVisible(nps);
    addAndMakeVisible(npr);
}

EnvelopeDisplay::~EnvelopeDisplay()
{
}

// Convert user input to envelope nodes
void EnvelopeDisplay::updateNodes()
{  
    //  [0] = OFF stage
    node[0].time.store(0.0f);   
    node[0].value.store(0.0f);

    node[1].value.store(scope_bounds.h - npa.y + gap);
    node[1].time.store(npa.x - gap);

    node[2].value.store(scope_bounds.h - npd.y + gap);
    node[2].time.store(npd.x - gap);

    node[3].value.store(scope_bounds.h - nps.y + gap);
    node[3].time.store(nps.x - gap);

    node[4].value.store(scope_bounds.h - npr.y + gap);
    node[4].time.store(npr.x - gap);

    node[5].time.store(scope_bounds.w);
    node[5].value.store(0.0f);
}

void EnvelopeDisplay::load()
{
    npa.y = area.getHeight() - node[1].value.load() - gap;
    npa.x = node[1].time.load() + gap;
    npa.setCentrePosition(npa.x, npa.y);

    npd.y = area.getHeight() - node[2].value.load() - gap;
    npd.x = node[2].time.load() + gap;
    npd.setCentrePosition(npd.x, npd.y);

    nps.y = area.getHeight() - node[3].value.load() - gap;
    nps.x = node[3].time.load() + gap;
    nps.setCentrePosition(nps.x, nps.y);

    npr.y = area.getHeight() - node[4].value.load() - gap;
    npr.x = node[4].time.load() + gap;
    npr.setCentrePosition(npr.x, npr.y);

    repaint();
}

void EnvelopeDisplay::paint (juce::Graphics& g)
{
    // Reset individual points left/right constraints
    npa.cl = gap;
    npa.cr = npd.x - gap;
    npd.cl = npa.x + gap;
    npd.cr = nps.x - gap;
    nps.cl = npd.x + gap;
    nps.cr = npr.x - gap;
    npr.cl = nps.x + gap;
    npr.cr = scope_bounds.w  + gap; 

    updateNodes();
    plot(g, 0.0f);
    g.setColour(colour);
    g.drawLine(npa.x, npa.y, npa.x, area.getHeight() - gap, 1.0f);
    g.drawLine(npd.x, npd.y, npd.x, area.getHeight() - gap, 1.0f);
    g.drawLine(nps.x, nps.y, nps.x, area.getHeight() - gap, 1.0f);
    g.drawLine(npr.x, npr.y, npr.x, area.getHeight() - gap, 1.0f);
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
    npa.cb = npd.cb = nps.cb = npr.cb = area.getHeight() - gap;

    npa.setBounds ( 20, 20, diameter, diameter);
    npa.cr = area.getWidth() / 4 - gap;
    npa.cl = gap;

    npd.setBounds ( 70, 40, diameter, diameter);
    npd.cr = area.getWidth() /2 - gap;
    npd.cl = area.getWidth() /4 + gap;

    nps.setBounds (120, 60, diameter, diameter);
    nps.cr = area.getWidth() /4 * 3 - gap;
    nps.cl = area.getWidth() /2 + gap;

    npr.setBounds (170, 80, diameter, diameter);
    npr.cr = area.getWidth()  - gap;
    npr.cl = area.getWidth() /4 * 3 + gap;

    updateNodes();
    repaint();
}



void EnvelopeDisplay::plot(juce::Graphics& g, float scale)
{
    sync();
    float h  = area.getHeight();
    
    static auto layer = juce::Image (juce::Image::PixelFormat::ARGB, scope_bounds.w, scope_bounds.h , true);
    static juce::Image::BitmapData bmp(layer, juce::Image::BitmapData::ReadWriteMode::readWrite);
    
    envd.generate(data.get(), scope_bounds.w);

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
    float incr = 1.0f;
    if( (event.x > 0) && (event.x < npa.x) )
    {
        node[1].curve.load() >= 3.0f ? node[1].curve.store(0.0f) : node[1].curve.store(envd.node[1].curve->load() + incr);
    }
    else if( (event.x > npa.x) && (event.x < npd.x) )
    {
        node[2].curve.load() >= 3.0f ? node[2].curve.store(0.0f) : node[2].curve.store(envd.node[2].curve->load() + incr);
    }
    else if( (event.x > npd.x) && (event.x < nps.x) )
    {
        node[3].curve.load() >= 3.0f ? node[3].curve.store(0.0f) : node[3].curve.store(envd.node[3].curve->load() + incr);
    }
    else if( (event.x > nps.x) && (event.x < npr.x) )
    {
        node[4].curve.load() >= 3.0f ? node[4].curve.store(0.0f) : node[4].curve.store(envd.node[4].curve->load() + incr);
    }
    else if( (event.x > npr.x) && (event.x < area.getWidth()) )
    {
        node[5].curve.load() >= 3.0f ? node[5].curve.store(0.0f) : node[5].curve.store(envd.node[5].curve->load() + incr);
    }
    repaint();
}
