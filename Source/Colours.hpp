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

inline juce::Colour cWhite         = juce::Colour::fromFloatRGBA (1.00f, 1.00f, 1.00f, 1.0f);
inline juce::Colour cGreen         = juce::Colour::fromFloatRGBA (0.34f, 0.74f, 0.66f, 1.0f);
inline juce::Colour cYellowDim     = juce::Colour::fromFloatRGBA (0.80f, 0.55f, 0.15f, 1.0f);
inline juce::Colour cFastBlack     = juce::Colour::fromFloatRGBA (0.10f, 0.10f, 0.10f, 1.0f);

inline juce::Colour cA             = juce::Colour::fromFloatRGBA (0.69f, 0.71f, 0.50f, 1.0f); // 0
inline juce::Colour cC             = juce::Colour::fromFloatRGBA (0.59f, 0.51f, 0.30f, 1.0f); // 2
inline juce::Colour cD             = juce::Colour::fromFloatRGBA (0.49f, 0.71f, 0.40f, 1.0f); // 3

inline juce::Colour cJackInOver    = juce::Colour::fromFloatRGBA (0.90f, 0.61f, 0.10f, 1.0f);
inline juce::Colour cJackInDown    = juce::Colour::fromFloatRGBA (0.80f, 0.51f, 0.00f, 1.0f);
inline juce::Colour cGreyLcd       = juce::Colour::fromFloatRGBA (0.50f, 0.50f, 0.50f, 1.0f);


inline const juce::Colour colour_set[] = 
{
    juce::Colour::fromFloatRGBA (1.09f, 0.71f, 0.20f, 1.0f), // 0 : Yellow
    juce::Colour::fromFloatRGBA (0.93f, 0.60f, 0.46f, 1.0f), // 1 : Coral
    juce::Colour::fromFloatRGBA (0.97f, 0.55f, 0.42f, 1.0f), // 2 : DarkPeach
    juce::Colour::fromFloatRGBA (0.76f, 0.91f, 0.55f, 1.0f), // 3 : PaleOlive
    juce::Colour::fromFloatRGBA (0.43f, 0.83f, 1.00f, 1.0f), // 4 : Blue
    juce::Colour::fromFloatRGBA (0.84f, 0.84f, 0.84f, 1.0f), // 5 : GreyPlastic
    juce::Colour::fromFloatRGBA (0.10f,	0.13f, 0.15f, 1.0f), // 6 : Almost Black
    juce::Colour::fromFloatRGBA (0.15f,	0.20f, 0.22f, 1.0f), // 7 : Charcoal grey
    juce::Colour::fromFloatRGBA (0.05f,	0.08f, 0.10f, 1.0f), // 8 : Mostly Black
    juce::Colour::fromFloatRGBA (0.15f,	0.32f, 0.32f, 1.0f), // 9 : Grey/Blue
    juce::Colour::fromRGB (0x05, 0x15,0x17),                 // 10: Asphalt grey
    juce::Colour::fromFloatRGBA (1.00f,	0.79f, 0.42f, 1.0f), // 11: Wheat

    juce::Colour::fromFloatRGBA (0.69f, 0.71f, 0.50f, 0.8f), // 12
    juce::Colour::fromFloatRGBA (0.69f, 0.31f, 0.40f, 0.8f), // 13
    juce::Colour::fromFloatRGBA (0.59f, 0.51f, 0.30f, 0.8f), // 14
    juce::Colour::fromFloatRGBA (0.49f, 0.71f, 0.40f, 0.8f), // 15
    juce::Colour::fromFloatRGBA (1.09f, 0.71f, 0.20f, 0.8f), // 16

    juce::Colour::fromFloatRGBA (0.69f, 0.71f, 0.50f, 0.8f), // 17
    juce::Colour::fromFloatRGBA (0.69f, 0.31f, 0.40f, 0.8f), // 18
    juce::Colour::fromFloatRGBA (0.59f, 0.51f, 0.30f, 0.8f), // 19
    juce::Colour::fromFloatRGBA (0.49f, 0.71f, 0.40f, 0.8f), // 20
    juce::Colour::fromFloatRGBA (1.09f, 0.71f, 0.20f, 0.8f), // 21

    juce::Colour::fromFloatRGBA (0.90f, 0.61f, 0.30f, 1.0f), // 22
    juce::Colour::fromFloatRGBA (0.80f, 0.71f, 0.40f, 1.0f), // 23
    juce::Colour::fromFloatRGBA (0.89f, 0.81f, 0.20f, 1.0f), // 24
    juce::Colour::fromFloatRGBA (0.79f, 0.61f, 0.40f, 1.0f), // 25
    

    juce::Colour::fromFloatRGBA (0.40f,	0.52f, 0.63f, 1.0f), // 26 


};


inline const juce::Colour cord_colour_set[] = 
{
    juce::Colour::fromRGB (0xFF, 0xFF, 0xFF)
};
