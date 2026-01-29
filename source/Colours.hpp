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
#include "juce_graphics/juce_graphics.h"
#include "juce_opengl/juce_opengl.h"
#include <JuceHeader.h>


namespace palette {
    inline const juce::Colour bg_normal         = juce::Colour::fromRGBA      ( 0x28,  0x35,  0x3C, 0xFF);
    inline const juce::Colour bg_dimmed         = juce::Colour::fromRGBA      ( 0x1F,  0x2A,  0x2F, 0xFF);
    inline const juce::Colour cord_normal       = juce::Colour::fromFloatRGBA (0.15f, 0.32f, 0.32f, 1.0f);
    inline const juce::Colour cord_highlighted  = juce::Colour::fromFloatRGBA (0.40f, 0.52f, 0.63f, 1.0f);
    inline const juce::Colour envelope          = juce::Colour::fromRGBA      ( 0x05,  0x15,  0x17, 0xFF);
    inline const juce::Colour marine            = juce::Colour::fromRGBA      ( 0x00,  0x96,  0x88, 0xFF);
    inline const juce::Colour rose_charcoal     = juce::Colour::fromFloatRGBA (0.69f, 0.31f, 0.40f, 0.8f);
    inline const juce::Colour transparent       = juce::Colour::fromRGBA      ( 0x00,  0x00,  0x00, 0x00);
    
    namespace cro {
        inline const GLfloat fg[4]  { 0.65f, 0.62f, 0.38f, 0.8f };
        inline const GLfloat bg[4]  { 0.12f, 0.16f, 0.18f, 0.8f };
    }

}
