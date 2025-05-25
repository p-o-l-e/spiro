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
#include "PluginProcessor.h"
#include "SpriteSlider.h"
#include "Display.h"
#include "ListSlider.h"
#include "blur.hpp"
#include "Display.h"
#include "Constraints.hpp"
#include "core/grid.hpp"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class Editor: public juce::AudioProcessorEditor, public juce::Timer, public EnvelopeDisplay::Listener, public Processor::Listener, public Display::Listener
{
    public:
        enum Sprite { Momentary, Radio, Slider };

		std::unique_ptr<Display>     display;
        juce::ImageComponent bg; 	 // Background layer

        void resized() override;
        void timerCallback() override;
        void visibilityChanged () override {};
        Editor (Processor&, juce::AudioProcessorValueTreeState&);
       ~Editor() override;

    private:
        std::unique_ptr<juce::Image> sprite[3][3];
        std::unique_ptr<juce::Image> bg_texture;

        juce::AudioProcessorValueTreeState& valueTreeState;
        std::unique_ptr<SpriteSlider[]> pot;
        std::unique_ptr<SliderAttachment> pot_attachment[0];//cell::settings::pot_n];
        std::vector<std::unique_ptr<juce::ImageButton>> button;
        std::unique_ptr<Sockets> sockets;
        int envelope_shown = 0;
        EnvelopeDisplay env[4];//cell::settings::env_n];
        Processor& processor;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
