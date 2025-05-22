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
#include "descriptor.hxx"
#include "grid.hpp"
#include "node.hpp"
#include "PluginEditor.h"
#include <iostream>

Editor::Editor(Processor& o, juce::AudioProcessorValueTreeState& tree): AudioProcessorEditor (&o), processor (o), valueTreeState (tree)
{
    std::cout<<"Editor::Editor()\n";
    // audioProcessor.suspendProcessing(true);
    pot_sprite[0]       = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::pot_48_png, BinaryData::pot_48_pngSize));
    pot_sprite[1]       = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::pot_32_png, BinaryData::pot_32_pngSize));
	pot_sprite[2]       = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::encoder_32_w_png, BinaryData::encoder_32_w_pngSize));

    btn_sprite[0][0]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::button_16_y_png, BinaryData::button_16_y_pngSize));
    btn_sprite[0][1]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::button_16_off_png, BinaryData::button_16_off_pngSize));
    btn_sprite[0][2]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::button_16_off_png, BinaryData::button_16_off_pngSize));

    btn_sprite[1][0]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::round_button_12_y_png, BinaryData::round_button_12_y_pngSize));
    btn_sprite[1][1]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::round_button_12_off_png, BinaryData::round_button_12_off_pngSize));
    btn_sprite[1][2]    = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::round_button_12_off_png, BinaryData::round_button_12_off_pngSize));

	bg_texture          = std::make_unique<juce::Image>(juce::ImageCache::getFromMemory(BinaryData::bg_png, BinaryData::bg_pngSize));
	
    bg.setImage(*bg_texture);
    bg.setOpaque(true);
    bg.setPaintingIsUnclipped(true);
    addAndMakeVisible(bg);

    core::grid.getSector(core::map::module::lfo, 0);
    // o.spiro.

    // display = std::make_unique<Display>(audioProcessor.c_buffer.get(), mwc.lcd_display.x, mwc.lcd_display.y, mwc.lcd_display.w, mwc.lcd_display.h);
    // pot = std::make_unique<SpriteSlider[]>(cell::settings::pot_n);

    // for(int i = 0; i < processor.spiro.rack.ccv(core::map::cv::c); ++i)
    // {
    //     interface::potentiometer_list p = static_cast<interface::potentiometer_list>(i);
    //
    //     pot[i].setPaintingIsUnclipped(true);
    //     pot[i].init(pot_sprite[(int)slider_list.at(p).type].get(), slider_list.at(p).type == slider_descriptor::type_t::Encoder);
    //     pot_attachment[i].reset (new SliderAttachment(valueTreeState, slider_list.at(p).id, pot[i]));
    //     addAndMakeVisible (pot[i]);
    // }
    //
    // for(int i = 0; i < cell::settings::btn_n; ++i)
    // {
    //     interface::button_list p = static_cast<interface::button_list>(i);
    //     auto bt = std::make_unique<juce::ImageButton>(button_list.at(p).name);
    //     button.emplace_back (std::move(bt));
    //
    //     button.at(i).get()->setPaintingIsUnclipped(true);
    //     button.at(i).get()->setClickingTogglesState(button_list.at(p).toggle);
    //     button.at(i).get()->setRadioGroupId(button_list.at(p).radio_id, juce::dontSendNotification);
    //     button.at(i).get()->setImages (false, false, true, *btn_sprite[(int)button_list.at(p).type][2], 1.0f, {}, 
    //                                              *btn_sprite[(int)button_list.at(p).type][1], 1.0f, {}, 
    //                                              *btn_sprite[(int)button_list.at(p).type][0], 1.0f, {});
    //
    //     addAndMakeVisible(button.at(i).get());
    // }



   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA
    * 
    * 
    **************************************************************************************************************************/


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


/*****************************************************************************************************************************
* 
*  On Resize
* 
******************************************************************************************************************************/
void Editor::resized()
{
	bg.setBounds(0, 0, mwc.width, mwc.height);


    // for(int i = 0; i < cell::settings::pot_n; ++i)
    // {
    //     interface::potentiometer_list p = static_cast<interface::potentiometer_list>(i);
    //     juce::Rectangle<int> r { slider_list.at(p).pos.x, slider_list.at(p).pos.y, slider_list.at(p).pos.w, slider_list.at(p).pos.h };
    //     pot[i].setBounds(r);
    // }


    processor.suspendProcessing(false);
}
