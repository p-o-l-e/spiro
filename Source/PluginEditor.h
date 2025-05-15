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

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SpriteSlider.h"
#include "Display.h"
#include "ListSlider.h"
#include "blur.hpp"
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
        void timerCallback() override { repaint(juce::Rectangle<int> { mwc.lcd_display.x, mwc.lcd_display.y, mwc.lcd_display.w, mwc.lcd_display.h }); };
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
        std::unique_ptr<SliderAttachment>               pot_attachment[core::settings::pot_n];
        std::vector<std::unique_ptr<juce::ImageButton>> button;
        
        EnvelopeDisplay                                 env[core::settings::env_n];
        SpiroSynth&                                     audioProcessor;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpiroSynthEditor)
};
