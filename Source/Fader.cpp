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
#include "Fader.h"
#include "Colours.hpp"


void SpiroLF::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{

}

juce::Label* SpiroLF::createSliderTextBox(juce::Slider &slider)
{
    auto localBounds = slider.getLocalBounds().toFloat();
    auto *l = new juce::Label();
    l->setJustificationType(juce::Justification::right );
    l->setInterceptsMouseClicks(false, false);
    l->setFont(juce::Font(11.0f));//localBounds.getHeight() * 0.8f));
    l->setColour(juce::Label::textColourId, colour_set[10]); 
    l->setColour(juce::Label::backgroundColourId, colour_set[13].withAlpha(0.8f)); 

    return l;
}

juce::Slider::SliderLayout SpiroLF::getSliderLayout(juce::Slider &slider)
{
    auto localBounds = slider.getLocalBounds().toFloat();
    juce::Slider::SliderLayout layout;
    auto textBounds = localBounds.withLeft(localBounds.getWidth() * 0.76f);
    layout.textBoxBounds = textBounds.toNearestInt();
    layout.sliderBounds = slider.getLocalBounds();
    return layout;
}


Fader::Fader()
{
    setLookAndFeel(&lf);
    setSliderStyle(juce::Slider::LinearHorizontal);
    setColour(juce::Slider::trackColourId, colour_set[27].withAlpha(0.85f)); // 13 - 3
    setColour(juce::Slider::textBoxBackgroundColourId, colour_set[10].withAlpha(0.25f));
    setColour(juce::Slider::textBoxOutlineColourId, colour_set[10].withAlpha(0.0f)); 

    setTextBoxIsEditable(false);

    setVelocityBasedMode(true);
    setVelocityModeParameters (0.1, 1, 0.09, false);
    setRange(0.1f, 9.999f, 0.001f);
    setSkewFactor(0.5f, false);
    setValue(1.0f);
    setDoubleClickReturnValue(true, 0.5f);
    setTextValueSuffix("s");
    setWantsKeyboardFocus(true);

}

Fader::~Fader()
{
    setLookAndFeel(nullptr);
}

void Fader::paint (juce::Graphics& g)
{
    g.fillAll(findColour(juce::Slider::textBoxBackgroundColourId));
    auto bounds = getLocalBounds().toFloat();
    auto h = bounds.getHeight();
    auto w = bounds.getWidth() * 0.75f;

    g.setColour(findColour (juce::Slider::trackColourId));
    auto c = (getValue() - getMinimum()) / getRange().getLength();
    g.fillRect
    (
        0.0f, 
        h - h / 3.0f, 
        c * w, 
        h
    );
}    

void Fader::mouseDown (const juce::MouseEvent& event)
{
    juce::Slider::mouseDown (event);
    setMouseCursor (juce::MouseCursor::NoCursor);
}

void Fader::mouseUp (const juce::MouseEvent& event)
{
    juce::Slider::mouseUp (event);
    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition (event.source.getLastMouseDownPosition());
    setMouseCursor (juce::MouseCursor::NormalCursor);
}

