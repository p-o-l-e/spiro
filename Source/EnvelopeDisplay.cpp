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

EnvelopeDisplay::EnvelopeDisplay (): A(this), D(this), S(this), R(this)
{
    node[1].curve.store(1.0f);
    node[2].curve.store(3.0f);
    node[3].curve.store(0.0f);
    node[4].curve.store(2.0f);
    node[5].curve.store(1.0f);

    for(int i = 0; i < SEGMENTS; ++i)
    {
        envd.node[i].time  = &node[i].time;
        envd.node[i].value = &node[i].value;
        envd.node[i].curve = &node[i].curve;
    }
    envd.time_scale = &core::one;
    envd.time_multiplier = 1.0f;
    envd.value_scale = 1.0f;

    addAndMakeVisible(A);
    addAndMakeVisible(D);
    addAndMakeVisible(S);
    addAndMakeVisible(R);
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

    node[1].value.store(scope_bounds.h - A.y + gap);
    node[1].time.store(A.x - gap);

    node[2].value.store(scope_bounds.h - D.y + gap);
    node[2].time.store(D.x - gap);

    node[3].value.store(scope_bounds.h - S.y + gap);
    node[3].time.store(S.x - gap);

    node[4].value.store(scope_bounds.h - R.y + gap);
    node[4].time.store(R.x - gap);

    node[5].time.store(scope_bounds.w);
    node[5].value.store(0.0f);
}

void EnvelopeDisplay::load()
{
    A.y = area.getHeight() - node[1].value.load() - gap;
    A.x = node[1].time.load() + gap;
    A.setCentrePosition(A.x, A.y);

    D.y = area.getHeight() - node[2].value.load() - gap;
    D.x = node[2].time.load() + gap;
    D.setCentrePosition(D.x, D.y);

    S.y = area.getHeight() - node[3].value.load() - gap;
    S.x = node[3].time.load() + gap;
    S.setCentrePosition(S.x, S.y);

    R.y = area.getHeight() - node[4].value.load() - gap;
    R.x = node[4].time.load() + gap;
    R.setCentrePosition(R.x, R.y);

    repaint();
}

void EnvelopeDisplay::paint (juce::Graphics& g)
{
    // Reset individual points left/right constraints
    A.cL = gap;
    A.cR = D.x - gap;
    D.cL = A.x + gap;
    D.cR = S.x - gap;
    S.cL = D.x + gap;
    S.cR = R.x - gap;
    R.cL = S.x + gap;
    R.cR = scope_bounds.w  + gap; 

    updateNodes();
    plot(g, 0.0f);
    g.setColour(colour);
    g.drawLine(A.x, A.y, A.x, area.getHeight() - gap, 1.0f);
    g.drawLine(D.x, D.y, D.x, area.getHeight() - gap, 1.0f);
    g.drawLine(S.x, S.y, S.x, area.getHeight() - gap, 1.0f);
    g.drawLine(R.x, R.y, R.x, area.getHeight() - gap, 1.0f);
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
    updateNodes();
    repaint();
}

void EnvelopeDisplay::plot(juce::Graphics& g, float scale)
{
    sync();
    float h  = area.getHeight();
    
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
    auto l = [this](const int p) 
    {
        node[p].curve.load() >= 3.0f ? node[p].curve.store(0.0f) : node[p].curve.store(envd.node[p].curve->load() + 1.0f);
    };
    if     ((event.x >   0) && (event.x < A.x)) l(1);
    else if((event.x > A.x) && (event.x < D.x)) l(2);
    else if((event.x > D.x) && (event.x < S.x)) l(3);
    else if((event.x > S.x) && (event.x < R.x)) l(4);
    else if((event.x > R.x) && (event.x < area.getWidth())) l(5);
    repaint();
}

void EnvelopeDisplay::setDefaults()
{
    A.setBounds(area.getWidth() / 14, area.getHeight() / 10, diameter, diameter);
    D.setBounds(area.getWidth() /  5, area.getHeight() /  3, diameter, diameter);
    S.setBounds(area.getWidth() /  3, area.getHeight() /  3, diameter, diameter);
    R.setBounds(area.getWidth() /  2, area.getHeight() /  2, diameter, diameter);
}
