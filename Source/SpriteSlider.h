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

#pragma once
#include <JuceHeader.h>


class SpriteSlider : public juce::Slider
{
	private:
		const juce::Image* sprite;
		int width;
	    int height;
	    int frames;
	    int x;
	    int y;
		float radius;
		juce::Rectangle<int> bounds;
        
    public:
		void resized() override;
        void paint(juce::Graphics& g) override;
        void init(const juce::Image*, const bool&);
		SpriteSlider(const juce::Image*, const bool&);
		SpriteSlider() {};
	   ~SpriteSlider() = default;
};
