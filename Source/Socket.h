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
#include <vector>
#include "modmatrix.hpp"
#include "Constraints.hpp"
#include "Colours.hpp"
#include "grid.hpp"
#include "primitives.hpp"

class Sockets: public juce::Component
{
    private:
        juce::Rectangle<int> area;
        float opacity   = 0.8f;
        bool  i_armed   = false;
        bool  o_armed   = false;
        bool  drag      = false;
        int   route     = -1;
        core::Socket* from_grid(int, bool);

    public:
        juce::Colour colour_normal = colour_set[9];
        juce::Colour colour_highlighted = colour_set[26];
        core::Patchbay* bay = nullptr;
        void drawMask(juce::Graphics&, juce::Colour);
        void drawCords(juce::Graphics&, float);
        void load();
        void mouseUp   (const juce::MouseEvent&) override;
        void mouseDrag (const juce::MouseEvent&) override;
        void mouseDown (const juce::MouseEvent&) override;
        void mouseMove (const juce::MouseEvent&) override;

        void resized() override;
        void paint(juce::Graphics& g) override;
        Sockets(const core::Rectangle<int>&, const core::Grid&);
       ~Sockets();
};
