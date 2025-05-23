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

#include "PluginProcessor.h"
#include "cro_interface.hpp"
#include "descriptor.hxx"
#include "grid.hpp"
#include "node.hpp"
#include "primitives.hpp"
#include "uid.hpp"
#include "PluginEditor.h"
#include <cstddef>
#include <iostream>

Editor::Editor(Processor& o, juce::AudioProcessorValueTreeState& tree): AudioProcessorEditor (&o), processor (o), valueTreeState (tree)
{
    std::cout<<"Editor::Editor()\n";
    // audioProcessor.suspendProcessing(true);
    pot_sprite[0]       = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::P48d_png, BinaryData::P48d_pngSize));
    pot_sprite[1]       = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::P32d_png, BinaryData::P32d_pngSize));
	pot_sprite[2]       = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::E32d_png, BinaryData::E32d_pngSize));

    btn_sprite[0][0]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::B16n_png, BinaryData::B16n_pngSize));
    btn_sprite[0][1]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::B16f_png, BinaryData::B16f_pngSize));
    btn_sprite[0][2]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::B16f_png, BinaryData::B16f_pngSize));

    btn_sprite[1][0]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::R12n_png, BinaryData::R12n_pngSize));
    btn_sprite[1][1]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::R12f_png, BinaryData::R12f_pngSize));
    btn_sprite[1][2]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::R12f_png, BinaryData::R12f_pngSize));

	bg_texture          = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::BGd_png, BinaryData::BGd_pngSize));
	
    bg.setImage(*bg_texture);
    bg.setOpaque(true);
    bg.setPaintingIsUnclipped(true);
    addAndMakeVisible(bg);

    core::grid.getSector(core::map::module::lfo, 0);
    // o.spiro.

    display = std::make_unique<Display>(processor.c_buffer.get(), mwc.lcd_display.x, mwc.lcd_display.y, mwc.lcd_display.w, mwc.lcd_display.h);
    pot = std::make_unique<SpriteSlider[]>(core::grid.pots);

    for(int i = 0; i < core::grid.pots; ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::slider);
        const core::Control* c = grid.control(uid);
        int type = 0;
        switch(c->flag) 
        {
            case core::map::flag::encoder: type = 2; break;
            case core::map::flag::B:  type = 1; break;
            default: break;
        }
        pot[i].setPaintingIsUnclipped(true);
        pot[i].init(pot_sprite[type].get(), c->flag & map::flag::encoder);
        // pot_attachment[i].reset (new SliderAttachment(valueTreeState, slider_list.at(p).id, pot[i]));
        addAndMakeVisible (pot[i]);
    }

    for(int i = 0; i < core::grid.buttons; ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::button);
        const core::Control* c = grid.control(uid);
    //     interface::button_list p = static_cast<interface::button_list>(i);
        auto bt = std::make_unique<juce::ImageButton>(c->postfix);
        button.emplace_back (std::move(bt));
        int type = 0;
        switch(c->flag) 
        {
            case core::map::flag::radio: type = 1; break;
            default: break;
        }

        button.at(i).get()->setPaintingIsUnclipped(true);
        button.at(i).get()->setClickingTogglesState(type);
        button.at(i).get()->setRadioGroupId(c->radio_id, juce::dontSendNotification);
        button.at(i).get()->setImages 
        (
            false, false, true, 
            *btn_sprite[type][2], 1.0f, {}, 
            *btn_sprite[type][1], 1.0f, {}, 
            *btn_sprite[type][0], 1.0f, {}
        );
        addAndMakeVisible(button.at(i).get());
    }

    display->setOpaque(true); // MUST
	display->setPaintingIsUnclipped(true);
    addAndMakeVisible(display.get());
    // addAndMakeVisible(audioProcessor.sockets.get());


    /***************************************************************************************************************************
    * 
    *  λ - Menu Button
    * 
    **************************************************************************************************************************/
    button.at(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::menu })).get()->onClick = [this]
    {
        std::cout<<"Main Menu button pressed...\n";
        stopTimer();
        display->MainMenu();
    };

    /***************************************************************************************************************************
    * 
    *  λ - Scope Button
    * 
    **************************************************************************************************************************/
    button.at(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::scope })).get()->onClick = [this]
    {
        std::cout<<"Scope button pressed...\n";

        startTimerHz(core::settings::scope_fps);
        display->page = Display::page_t::scope;
        display->layer_on = false;
    };

    setResizable(false, false);
    setSize (mwc.width, mwc.height);
    setWantsKeyboardFocus (true);
} 

/*****************************************************************************************************************************
* 
* Destructor
* 
******************************************************************************************************************************/
Editor::~Editor()
{
    stopTimer();
}

void Editor::timerCallback()  
{ 
    repaint(juce::Rectangle<int> { mwc.lcd_display.x, mwc.lcd_display.y, mwc.lcd_display.w, mwc.lcd_display.h }); 
}

/*****************************************************************************************************************************
* 
*  On Resize
* 
******************************************************************************************************************************/
void Editor::resized()
{
	bg.setBounds(0, 0, mwc.width, mwc.height);

    for(int i = 0; i < core::grid.pots; ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::slider);
        auto bounds = grid.getBounds(uid);

        juce::Rectangle<int> r 
        { 
            static_cast<int>(bounds->x + grid.bounds.x), 
            static_cast<int>(bounds->y + grid.bounds.y), 
            static_cast<int>(bounds->w), 
            static_cast<int>(bounds->h) 
        };
        pot[i].setBounds(r);
    }
    for(int i = 0; i < core::grid.buttons; ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::button);
        auto bounds = grid.getBounds(uid);
        juce::Rectangle<int> r 
        { 
            static_cast<int>(bounds->x + grid.bounds.x), 
            static_cast<int>(bounds->y + grid.bounds.y), 
            static_cast<int>(bounds->w), 
            static_cast<int>(bounds->h) 
        };
        button.at(i).get()->setBounds(r);
    }
    display->setBounds(mwc.lcd_display.x, mwc.lcd_display.y, mwc.lcd_display.w, mwc.lcd_display.h);
    startTimerHz(core::settings::scope_fps);



    processor.suspendProcessing(false);
}
