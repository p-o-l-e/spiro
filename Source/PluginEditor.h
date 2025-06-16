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

#include "PluginProcessor.h"
#include "SpriteSlider.h"
#include "BarSlider.h"
#include "EnvelopeDisplay.h"
#include "Display.h"
#include <cstdint>
#include <memory>
#include <vector>

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class Editor: public juce::AudioProcessorEditor, public juce::Timer, /*public EnvelopeDisplay::Listener,*/ public Processor::Listener, public Display::Listener
{
    public:
        enum Sprite { Momentary, Radio, Slider };
		std::unique_ptr<Display> display;
        bool fade = false;
        Processor& processor;
        void resized() override;
        void timerCallback() override;
        void visibilityChanged() override {};
        Editor(Processor&, juce::AudioProcessorValueTreeState&);
       ~Editor() override;

    private:
        void saveMatrix();
        void loadMatrix();
        void clearMatrix();
        void saveCall() override;
        void loadCall() override;
        void resetCall() override;
        void setOption(const core::uid_t&, const float, const float);
        void switchEnvelope(uint8_t);
        std::unique_ptr<juce::Image> sprite[3][3];
        std::unique_ptr<juce::Image> bg_texture;
        juce::ImageComponent bg;
        juce::AudioProcessorValueTreeState& valueTreeState;
        std::unique_ptr<SpriteSlider[]> slider;
        std::unique_ptr<Fader[]> fader;
        int sliders;
        int faders;
        std::vector<std::unique_ptr<juce::ImageButton>> button;
        std::vector<std::unique_ptr<ButtonAttachment>> buttonAttachment;
        std::vector<std::unique_ptr<SliderAttachment>> sliderAttachment;
        const int envn = 4;
        int envelope_shown = 0;
        std::vector<std::unique_ptr<EnvelopeDisplay>> env;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
