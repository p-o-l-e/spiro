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
#include "ListSlider.h"


NumberBox::NumberBox()
{
    setSliderStyle (juce::Slider::LinearBar);
    setColour (juce::Slider::trackColourId, juce::Colours::transparentWhite);
    setTextBoxIsEditable (false);
    setVelocityBasedMode (true);
    setVelocityModeParameters (0.1, 1, 0.09, false);
    setRange (0.0f, 1.0f, 0.001f);
    setValue (0.0);
    setDoubleClickReturnValue (true, 0.5f);
    setTextValueSuffix ("");
    setWantsKeyboardFocus (true);

}

NumberBox::~NumberBox(){}

void NumberBox::paint (juce::Graphics& g)
{
    g.fillAll(findColour (juce::Slider::textBoxBackgroundColourId));
    auto bounds = getLocalBounds().toFloat();
    auto h = bounds.getHeight();
    auto w = bounds.getWidth();
    // g.setColour (findColour (juce::Slider::textBoxOutlineColourId));

    g.setColour (findColour (juce::Slider::trackColourId));
    g.drawLine(2.0f, 0.0f, 2.0f, h, 4.0f);

    auto c = (getValue()-getMinimum())/getRange().getLength();
    auto l = h / 8.0f;

    g.fillRect( w - w / 8.0f, 
                h - c * (h - l) - l, 
                w, 
                l
                );
}

void NumberBox::mouseDown (const juce::MouseEvent& event)
{
    juce::Slider::mouseDown (event);
    setMouseCursor (juce::MouseCursor::NoCursor);
}

void NumberBox::mouseUp (const juce::MouseEvent& event)
{
    juce::Slider::mouseUp (event);
    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition (event.source.getLastMouseDownPosition());
    setMouseCursor (juce::MouseCursor::NormalCursor);
}

// void NumberBox::mouseEnter (const juce::MouseEvent& event)
// {
//     setColour(juce::Slider::textBoxOutlineColourId, hoveredColour); // 3 : PaleOlive
//     repaint();
// }

// void NumberBox::mouseExit (const juce::MouseEvent& event)
// {
//     setColour(juce::Slider::textBoxOutlineColourId, normalColour);
//     repaint();
// }
