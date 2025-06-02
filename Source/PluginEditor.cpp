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

#include "Constraints.hpp"
#include "PluginProcessor.h"
#include "SpriteSlider.h"
#include "cro_interface.hpp"
#include "cso_interface.hpp"
#include "descriptor.hxx"
#include "env_interface.hpp"
#include "grid.hpp"
#include "juce_gui_basics/juce_gui_basics.h"
#include "node.hpp"
#include "primitives.hpp"
#include "uid.hpp"
#include "Menu.hpp"
#include "PluginEditor.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <sys/types.h>

Editor::Editor(Processor& o, juce::AudioProcessorValueTreeState& tree): AudioProcessorEditor (&o), processor (o), valueTreeState (tree)
{
    std::cout<<"-- Constructing Editor...\n";
    processor.suspendProcessing(true);
   /***************************************************************************************************************************
    * 
    *  Sprites and Textures
    * 
    **************************************************************************************************************************/
    sprite[Sprite::Momentary][0] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::B16n_png, BinaryData::B16n_pngSize));
    sprite[Sprite::Momentary][1] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::B16f_png, BinaryData::B16f_pngSize));
    sprite[Sprite::Momentary][2] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::B16f_png, BinaryData::B16f_pngSize));

    sprite[Sprite::Radio][0] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::R12n_png, BinaryData::R12n_pngSize));
    sprite[Sprite::Radio][1] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::R12f_png, BinaryData::R12f_pngSize));
    sprite[Sprite::Radio][2] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::R12f_png, BinaryData::R12f_pngSize));

    sprite[Sprite::Slider][0] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::P48d_png, BinaryData::P48d_pngSize));
    sprite[Sprite::Slider][1] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::P32d_png, BinaryData::P32d_pngSize));
	sprite[Sprite::Slider][2] = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::E32d_png, BinaryData::E32d_pngSize));

	bg_texture = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::BGd_png, BinaryData::BGd_pngSize));
   
    bg.setImage(*bg_texture);
    bg.setOpaque(true);
    bg.setPaintingIsUnclipped(true);
    addAndMakeVisible(bg);

    display = std::make_unique<Display>(processor.buffer, core::constraints::oled);
   
   /***************************************************************************************************************************
    * 
    *  Sliders initialization
    * 
    **************************************************************************************************************************/
    slider = std::make_unique<SpriteSlider[]>(core::grid.count(core::Control::slider));
    sliderAttachment.reserve(core::grid.count(core::Control::slider));

    for(int i = 0; i < core::grid.count(core::Control::slider); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::slider);
        const core::Control* c = core::grid.control(uid);
        int type = 0;
        switch(c->flag) 
        {
            case core::map::flag::encoder: type = 2; break;
            case core::map::flag::B:  type = 1; break;
            default: break;
        }
        // slider[i]new SpriteSlider {});
        slider[i].setPaintingIsUnclipped(true);
        slider[i].init(sprite[Sprite::Slider][type].get(), c->flag & core::map::flag::encoder);
        // std::cout<<core::grid.name(uid, true)<<"\n";
        sliderAttachment.emplace_back(std::make_unique<SliderAttachment>(valueTreeState, core::grid.name(uid, true), slider[i]));
        addAndMakeVisible(slider[i]);
    }

   /***************************************************************************************************************************
    * 
    *  Buttons initialization
    * 
    **************************************************************************************************************************/
    button.reserve(core::grid.count(core::Control::button));
    buttonAttachment.reserve(core::grid.count(core::Control::button));

    for(int i = 0; i < core::grid.count(core::Control::button); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::button);
        const core::Control* c = core::grid.control(uid);
        button.emplace_back(std::make_unique<juce::ImageButton>(c->postfix));
        auto type = Sprite::Momentary;
        switch(c->flag) 
        {
            case core::map::flag::radio: type = Sprite::Radio; break;
            default: break;
        }

        button[i]->setPaintingIsUnclipped(true);
        button[i]->setClickingTogglesState(type);
        button[i]->setRadioGroupId(c->radio_id, juce::dontSendNotification);
        button[i]->setImages 
        (
            false, false, true, 
            *sprite[type][2], 1.0f, {}, 
            *sprite[type][1], 1.0f, {}, 
            *sprite[type][0], 1.0f, {}
        );
        buttonAttachment.emplace_back(std::make_unique<ButtonAttachment>(valueTreeState, core::grid.name(uid, true), *button[i]));
        addAndMakeVisible(button[i].get());
    }
    
    for(int i = 0; i < 4; ++i)
    {
        env[i].id = i;
        env[i].addListener(this);
        env[i].setOpaque(true);
        env[i].setPaintingIsUnclipped(true);
        addAndMakeVisible(env[i]);
    }
    processor.addListener(this);
    display->addListener(this);

    display->setOpaque(true); // MUST
	display->setPaintingIsUnclipped(true);

    addAndMakeVisible(display.get());
    addAndMakeVisible(processor.sockets.get());

   /***************************************************************************************************************************
    * 
    *  λ
    * 
    **************************************************************************************************************************/
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::menu }))]->onClick = [this]
    {
        mainMenu(this);
    };

    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::scope }))]->onClick = [this]
    {
        croMenu(this, 0);
    };
    
   /***************************************************************************************************************************
    * 
    *  λ - VCOs
    * 
    **************************************************************************************************************************/    
    for(int i = 0; i < core::grid.count(core::map::module::vco); ++i)
    {
        button[(core::grid.getIndex(core::uid_t{ core::map::module::vco, i, core::map::cv::c, core::vco::ctl::options }))]->onClick = [this, i]
        {
            vcoMenu(this, i);
        };
    }

   /***************************************************************************************************************************
    * 
    *  λ - Dynamic Systems
    * 
    **************************************************************************************************************************/    
    for(int i = 0; i < core::grid.count(core::map::module::cso); ++i)
    {
        button[(core::grid.getIndex(core::uid_t{ core::map::module::cso, i, core::map::cv::c, core::cso::ctl::options }))]->onClick = [this, i]
        {
            csoMenu(this, i);
        };
    }

   /***************************************************************************************************************************
    * 
    *  λ - LFOs
    * 
    **************************************************************************************************************************/    
    for(int i = 0; i < core::grid.count(core::map::module::lfo); ++i)
    {
        button[(core::grid.getIndex(core::uid_t{ core::map::module::lfo, i, core::map::cv::c, core::lfo::ctl::options }))]->onClick = [this, i]
        {
            lfoMenu(this, i);
        };
    }
    
   /***************************************************************************************************************************
    * 
    *   λ - Soft A (Cancel or Rewind)
    * 
    **************************************************************************************************************************/
    // button.at(interface::button_list::soft_a).get()->onClick = [this]
    // {
    //     float v;
    //     switch(display->page)
    //     {
    //         case Display::Page::scope: scopeOptions(0, false); break;
    //         case Display::Page::CsoA: setOption(uid_t { core::map::module::cso, 0, core::map::cv::c, core::cso::ctl::form }, 1.0f, 4.0f }; break;
    //         case Display::Page::CsoB: setOption; break;
    //         case Display::Page::LfoA: lfoOptions; break;
    //         case Display::Page::LfoB: lfoOptions; break;
    //         case Display::Page::VcoA: vcoOptions; break;
    //         case Display::Page::VcoB: vcoOptions; break;
    //         case Display::Page::VcoC: vcoOptions; break;
    //         case Display::Page::VcoD: vcoOptions; break;
    //         case Display::Page::load:          
    //             stopTimer();
    //             display->MainMenu();
    //             break;
    //
    //         case Display::page_t::save:          
    //             stopTimer();
    //             display->MainMenu();
    //             break;
    //
    //         case Display::page_t::menu:          
    //             startTimerHz(core::settings::scope_fps);
    //             display->page = Display::page_t::scope;
    //             display->layer_on = false;
    //             button.at(interface::button_list::scope).get()->setToggleState(true, juce::NotificationType::dontSendNotification);
    //             break;
    //
    //         default:
    //             break;
    //     }
    //     display->repaint();
    // };

    setResizable(false, false);
    setSize (core::constraints::W, core::constraints::H);
    setWantsKeyboardFocus (true);

    processor.suspendProcessing(false);

    startTimerHz(core::settings::scope_fps);
    std::cout<<"-- Editor created\n";

} 

/*****************************************************************************************************************************
* 
* Destructor
* 
******************************************************************************************************************************/
Editor::~Editor()
{
    stopTimer();
    std::cout<<"-- Editor deconstructed\n";

}

void Editor::timerCallback()  
{ 
    repaint(juce::Rectangle<int>
    {
        core::constraints::oled.x,
        core::constraints::oled.y, 
        core::constraints::oled.w,
        core::constraints::oled.h
    }); 
}

/*****************************************************************************************************************************
* 
*  On Resize
* 
******************************************************************************************************************************/
void Editor::resized()
{
    std::cout<<"-- Resizing Editor...\n";

    processor.suspendProcessing(true);

	bg.setBounds(0, 0, core::constraints::W, core::constraints::H);

    for(int i = 0; i < core::grid.count(core::Control::slider); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::slider);
        auto bounds = core::grid.getBounds(uid);

        juce::Rectangle<int> r 
        { 
            static_cast<int>(bounds.x + core::constraints::gap_x), 
            static_cast<int>(bounds.y + core::constraints::gap_y), 
            static_cast<int>(bounds.w), 
            static_cast<int>(bounds.h) 
        };
        slider[i].setBounds(r);
    }

    for(int i = 0; i < core::grid.count(core::Control::button); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::button);
        auto bounds = core::grid.getBounds(uid);
        juce::Rectangle<int> r 
        { 
            static_cast<int>(bounds.x + core::constraints::gap_x), 
            static_cast<int>(bounds.y + core::constraints::gap_y), 
            static_cast<int>(bounds.w), 
            static_cast<int>(bounds.h) 
        };
        button[i]->setBounds(r);
    }

    for(int i = 0; i < 4; ++i)
    {
        juce::Rectangle<int> r 
        {
            core::constraints::envd.x,
            core::constraints::envd.y,
            core::constraints::envd.w,
            core::constraints::envd.h 
        };
        env[i].setBounds(r);
    }

    env[0].setVisible(true);
    env[1].setVisible(false);
    env[2].setVisible(false);
    env[3].setVisible(false);

    processor.sockets->setBounds(juce::Rectangle<int> 
    {
        core::constraints::pbay.x,
        core::constraints::pbay.y,
        core::constraints::pbay.w,
        core::constraints::pbay.h 
    });

    display->setBounds
    (
       core::constraints::oled.x,
       core::constraints::oled.y,
       core::constraints::oled.w,
       core::constraints::oled.h
    );


    std::cout<<"-- Editor resized\n";
}
