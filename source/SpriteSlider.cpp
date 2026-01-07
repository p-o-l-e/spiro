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
        g.drawImage(*sprite, (int)x, (int)y, 2 * (int)radius, 2 * (int)radius, 0, frame_id * width, width, width);
    }
}

void SpriteSlider::init(const juce::Image* img, const bool& encoder)
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


SpriteSlider::SpriteSlider(const juce::Image* img, const bool& enc)
{   
    init(img, enc);
}
