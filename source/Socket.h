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
#include "JuceHeader.h"
#include "modmatrix.hpp"
#include "Constraints.hpp"
#include "Colours.hpp"
#include "grid.hpp"

class Sockets: public juce::Component
{
    private:
        juce::Rectangle<int> area;
        float opacity = 0.8f;
        bool sourceArmed = false;
        bool targetArmed = false;
        core::Socket* fromModMatrix(int, bool);
        juce::MouseCursor cursor;

    public:
        juce::Colour colourNormal = palette::cord_normal;
        juce::Colour colourHighlighted = palette::cord_highlighted;
        core::Patchbay* bay = nullptr;
        void drawMask(juce::Graphics&);
        void drawCords(juce::Graphics&, float);
        void mouseUp(const juce::MouseEvent&) override;
        void mouseDrag(const juce::MouseEvent&) override;
        void mouseDown(const juce::MouseEvent&) override;
        void mouseMove(const juce::MouseEvent&) override;

        void load();
        void resized() override;
        void paint(juce::Graphics& g) override;
        Sockets(const core::Rectangle<int>&, const core::Grid&);
       ~Sockets() override;
};
