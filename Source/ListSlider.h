#pragma once

#include <JuceHeader.h>


class NumberBox: public juce::Slider
{
    public:
        float mod = 0.0f;
        float eax;
        float range;
        float centre;

        void paint (juce::Graphics& g) override;
        void mouseDown (const juce::MouseEvent& event) override;
        void mouseUp (const juce::MouseEvent& event) override;
       
        NumberBox();
       ~NumberBox();
};
