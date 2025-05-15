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

#include "SpriteSlider.h"


void SpriteSlider::resized()
{
    bounds = getLocalBounds();
    radius = juce::jmin((float)bounds.getWidth() * 0.5f, (float)bounds.getHeight() * 0.5f);

    x = roundf((float)bounds.getX() + (float)bounds.getWidth()  * 0.5f - radius - 1.0f);
    y = roundf((float)bounds.getY() + (float)bounds.getHeight() * 0.5f - radius);
}

void SpriteSlider::paint(juce::Graphics& g)
{
    if (sprite->isValid())
    {
        double rotation = (getValue() - getMinimum())/(getMaximum() - getMinimum());
        int frame_id = (int)round(rotation * ((double)frames - 1.0));
        g.drawImage(*sprite, (int)x, (int)y, 2 * (int)radius, 2 * (int)radius, 0, frame_id*width, width, width);
    }
}

void SpriteSlider::init(const juce::Image* img, bool encoder)
{
    sprite = img;
    width  = sprite->getWidth();
    height = sprite->getHeight();
    frames = height / width;
    setSliderStyle(juce::Slider:: RotaryVerticalDrag);
    setRotaryParameters (0, 1, !encoder);
    setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    setTextBoxIsEditable(false);
    setVelocityBasedMode(true);
    setVelocityModeParameters (0.1, 1, 0.09, false);
    setRange(0.0f, 1.0f, 0.001f);
    setValue(0.0);
    setDoubleClickReturnValue(true, 0.0f);
    // setBufferedToImage(true);
}


SpriteSlider::SpriteSlider(const juce::Image* img)
{   
    init(img, false);
}
