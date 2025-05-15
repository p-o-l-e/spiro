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
#include <iostream>
#include "ListSlider.h"
#include "Colours.hpp"
#include "blur.hpp"
#include "env.hpp"
#include "containers.hpp"
#include "utility.hpp"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
struct b_point
{
    std::atomic<float> time  { 0.0f };
    std::atomic<float> value { 0.0f };
    std::atomic<float> curve { 0.0f };
};

class EnvelopeDisplay  : public juce::Component
{
    public:
        int   id;
        int   gap = 5;                              // = Dragger radius
        int   diameter = gap * 2 + 1;
        float opacity = 0.2f;
        float curve_width = 2.0f;
        core::envelope              envd;           // Displayed envelope
        juce::Rectangle<int>        area;           // Display bounds
        core::rectangle<int>        scope_bounds;   // Scope constraints
        b_point                     node[SEGMENTS]; // Input nodes
        std::unique_ptr<float[]>    data;
        juce::Colour colour = colour_set[10];
        void plot(juce::Graphics&, float);
        void load();                                // Load from memory
        void updateNodes();
        void paint (juce::Graphics& g) override;
        void resized() override;
       /***********************************************************************************************************************
        * Node handler
        **********************************************************************************************************************/
        struct  NodePoint : public juce::Component
        {
            juce::ComponentDragger node;
            juce::ComponentBoundsConstrainer constrainer;
            EnvelopeDisplay* parent;
            juce::Rectangle<int> area;
            int  x,  y;
            int cl, cr;                             // Left/Right constrain
            int ct, cb;                             // Top/Bottom constrain

            void paint (juce::Graphics& g) override
            {
                g.setColour   (parent->colour);
                g.fillEllipse (area.toFloat());
            }

            void mouseDown (const juce::MouseEvent& event) override
            {
                node.startDraggingComponent (this, event);
            }

            void mouseDrag (const juce::MouseEvent& event) override
            {
                node.dragComponent (this, event, &constrainer);
                x = getX() + getWidth () / 2;
                y = getY() + getHeight() / 2;

                if(x >= cr) setCentrePosition(x = cr, y);
                if(x <= cl) setCentrePosition(x = cl, y);
                if(y >= cb) setCentrePosition(x, y = cb);

                parent->envd.regenerate = true;
                parent->repaint();
            }

            void mouseUp (const juce::MouseEvent&) override 
            { 
                parent->listeners.call([this](Listener &l) { l.envChanged(parent->id); });
            };
            NodePoint(EnvelopeDisplay* p): parent(p)
            {
                auto d = parent->diameter;
                constrainer.setMinimumOnscreenAmounts (d, d, d, d);
            }

            void resized() override
            {
                area = getLocalBounds();
                x = getX() + getWidth()  / 2;
                y = getY() + getHeight() / 2;
                parent->repaint();
            }
        };

        NodePoint npa;
        NodePoint npd;
        NodePoint nps;
        NodePoint npr;
       /***********************************************************************************************************************
        * Listener
        **********************************************************************************************************************/
        class Listener 
        {
            public:
                virtual ~Listener() = default;
                virtual void envChanged(int) {};

        };
        void addListener(Listener *l)       { listeners.add(l);     }
        void removeListener(Listener *l)    { listeners.remove(l);  }
       /***********************************************************************************************************************
        * 
        **********************************************************************************************************************/
        void mouseUp   (const juce::MouseEvent&) override 
        { 
            listeners.call([this](Listener &l) { l.envChanged(id); });
        };
        void mouseDown (const juce::MouseEvent&) override;

        EnvelopeDisplay();
       ~EnvelopeDisplay() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeDisplay)
        juce::ListenerList<Listener> listeners;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
