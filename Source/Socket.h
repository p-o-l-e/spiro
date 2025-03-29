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
#include <vector>
#include "cell/modmatrix.hpp"
#include "Constraints.hpp"
#include "Colours.hpp"

class Sockets: public juce::Component
{
    private:
        juce::Rectangle<int> area;
        float opacity   = 0.8f;
        bool  i_armed   = false;
        bool  o_armed   = false;
        bool  drag      = false;
        int   route     = -1;
        cell::socket* from_grid(int, bool);

    public:
        juce::Colour colour_normal = colour_set[9];
        juce::Colour colour_highlighted = colour_set[26];
        cell::patchbay* bay = nullptr;
        void drawMask(juce::Graphics&, juce::Colour);
        void drawCords(juce::Graphics&, float);
        void load();
        void mouseUp   (const juce::MouseEvent&) override;
        void mouseDrag (const juce::MouseEvent&) override;
        void mouseDown (const juce::MouseEvent&) override;
        void mouseMove (const juce::MouseEvent&) override;

        void resized () override;
        void paint (juce::Graphics& g) override;
        Sockets(int, int, int, int);
       ~Sockets();
};
