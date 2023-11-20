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
#include "cell/blur.hpp"
#include "Display.h"
#include "Constraints.hpp"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class SpiroSynthEditor: public juce::AudioProcessorEditor, 
                        public juce::Timer, 
                        public EnvelopeDisplay::Listener, 
                        public SpiroSynth::Listener,
                        public Display::Listener
{
    public:
		std::unique_ptr<Display>     display;
        std::unique_ptr<juce::Image> pot_sprite[3];
        std::unique_ptr<juce::Image> btn_sprite[2][3];
        std::unique_ptr<juce::Image> bg_texture;

        juce::ImageComponent bg; 	 // Background layer
        main_window_constraints mwc;

        void chsOptions(int, bool);
        void scopeOptions(int, bool);
        void lfoOptions(int, bool);
        void vcoOptions(int, bool);
        void envOptions(float, bool);
        void loadMatrix();
        void saveMatrix();
        void clearMatrix();
        void envBroadcast(int);
        void envLoad();
        void envSwitch(int);
        void onReload();
        void resized() override;
        void timerCallback() override { repaint(mwc.lcd_display); };
        void envChanged(int) override;
        void bufferDisconnected() override;
        void visibilityChanged () override { loadMatrix(); };
        void saveCall() override;
        void loadCall() override;
        SpiroSynthEditor (SpiroSynth&, juce::AudioProcessorValueTreeState&);
       ~SpiroSynthEditor() override;

    private:
        juce::AudioProcessorValueTreeState&             valueTreeState;
        int                                             envelope_shown = 0;
        std::unique_ptr<SpriteSlider[]>                 pot;
        std::unique_ptr<SliderAttachment>               pot_attachment[cell::settings::pot_n];
        std::vector<std::unique_ptr<juce::ImageButton>> button;
        
        EnvelopeDisplay                                 env[cell::settings::env_n];
        SpiroSynth&                                     audioProcessor;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpiroSynthEditor)
};
