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
#include "PluginEditor.h"
#include "Display.h"
#include <iostream>
#include <memory>

Editor::Editor(Processor& o, juce::AudioProcessorValueTreeState& tree): AudioProcessorEditor(&o), processor(o), valueTreeState(tree)
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
        slider[i].setPaintingIsUnclipped(true);
        slider[i].init(sprite[Sprite::Slider][type].get(), c->flag & core::map::flag::encoder);
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
   
    for(int i = 0; i < envn; ++i)
    {
        env.emplace_back(std::make_unique<EnvelopeDisplay>(&processor, i));
        env[i].get()->setOpaque(true);
        env[i].get()->setPaintingIsUnclipped(true);
        addAndMakeVisible(env[i].get());
    }
    processor.addListener(this);

    resetCall();
    addAndMakeVisible(processor.sockets.get());

   /***************************************************************************************************************************
    * 
    *  λ - Main Menu
    * 
    **************************************************************************************************************************/
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::menu }))]->onClick = [this]
    {
        fade = true;
        display->page = Display::Page::MainMenu;
        display->mainMenu();
    };

   /***************************************************************************************************************************
    * 
    *  λ - Scope
    * 
    **************************************************************************************************************************/
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::scope }))]->onClick = [this]
    {
        startTimerHz(core::settings::scope_fps);
        fade = false;
        display->page = Display::Page::CroA;
        display->croMenu();
        // display->layerOn = false;
    };
    
   /***************************************************************************************************************************
    * 
    *  λ - VCOs
    * 
    **************************************************************************************************************************/    
    for(uint8_t i = 0; i < core::grid.count(core::map::module::vco); ++i)
    {
        button[(core::grid.getIndex(core::uid_t{ core::map::module::vco, i, core::map::cv::c, core::vco::ctl::options }))]->onClick = [this, i]
        {
            fade = true;
            display->page = static_cast<Display::Page>(Display::VcoA + i);
            display->moduleMenu(&processor.spiro, core::map::module::vco, i);
        };
    }

   /***************************************************************************************************************************
    * 
    *  λ - Dynamic Systems
    * 
    **************************************************************************************************************************/    
    for(uint8_t i = 0; i < core::grid.count(core::map::module::cso); ++i)
    {
        button[(core::grid.getIndex(core::uid_t{ core::map::module::cso, i, core::map::cv::c, core::cso::ctl::options }))]->onClick = [this, i]
        {
            fade = true;
            display->page = static_cast<Display::Page>(Display::CsoA + i);
            display->moduleMenu(&processor.spiro, core::map::module::cso, i);
        };
    }

   /***************************************************************************************************************************
    * 
    *  λ - LFOs
    * 
    **************************************************************************************************************************/    
    for(uint8_t i = 0; i < core::grid.count(core::map::module::lfo); ++i)
    {
        button[(core::grid.getIndex(core::uid_t{ core::map::module::lfo, i, core::map::cv::c, core::lfo::ctl::options }))]->onClick = [this, i]
        {
            fade = true;
            display->page = static_cast<Display::Page>(Display::LfoA + i);
            display->moduleMenu(&processor.spiro, core::map::module::lfo, i);
        };
    }

    /***************************************************************************************************************************
    * 
    *  λ - Envelope switches
    * 
    **************************************************************************************************************************/    
    for(uint8_t i = 0; i < core::grid.count(core::map::module::env); ++i)
    {
        button[(core::grid.getIndex(core::uid_t{ core::map::module::env, i, core::map::cv::c, core::env::ctl::select }))]->onClick = [this, i]
        {
            switchEnvelope(i);
        };
    }
   /***************************************************************************************************************************
    * 
    *  λ - Soft A (Jump Up)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::sa }))]->onClick = [this]
    {
        display->row[display->page] = 0;
        display->switchPage(&processor, display->page);
    };

   /***************************************************************************************************************************
    * 
    *  λ - Soft B (Step Up)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::sb }))]->onClick = [this]
    {
        display->row[display->page]--;
        display->switchPage(&processor, display->page);
    };

   /***************************************************************************************************************************
    * 
    *  λ - Soft C (Step Down)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::sc }))]->onClick = [this]
    {
        display->row[display->page]++;
        display->switchPage(&processor, display->page);
    };

   /***************************************************************************************************************************
    * 
    *  λ - Soft D (Jump Down)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::sd }))]->onClick = [this]
    {
        display->row[display->page] = display->rows_max;
        display->switchPage(&processor, display->page);
    };

   /***************************************************************************************************************************
    * 
    *  λ - Soft E (Jump Left) (Step Left for Scope) (Cancel for Menu)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::se }))]->onClick = [this]
    {
        if(display->page == Display::Page::CroA)
        {
            auto uid = core::uid_t { core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::type };
            auto control = core::grid.control(uid);
            setOption(uid, -control->step, control->max);
        }
        else if(display->page == Display::Page::MainMenu)
        {
            startTimerHz(core::settings::scope_fps);
            fade = false;
            display->page = Display::Page::CroA;
            display->croMenu();
        }
        else if(display->page == Display::Page::Save) display->mainMenu();
        else if(display->page == Display::Page::Load) display->mainMenu();
        else 
        {        
            auto control = core::grid.control(display->uid);
            setOption(display->uid, -control->max, control->max);
            display->switchPage(&processor, display->page);
        }
    };

   /***************************************************************************************************************************
    * 
    *  λ - Soft F (Step Left) (Step Right for Scope) (Ok for Menu)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::sf }))]->onClick = [this]
    {
        if(display->page == Display::Page::CroA)
        {
            auto uid = core::uid_t { core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::type };
            auto control = core::grid.control(uid);
            setOption(uid, control->step, control->max);
        }
        else if(display->page == Display::Page::MainMenu)
        {
            if(display->row[display->page] == 0)
            {
                startTimerHz(core::settings::scope_fps);
                display->page = Display::Page::Save;
                display->saveMenu();
                display->grabKeyboardFocus();
                display->inputBox.setText(processor.currentPresetName);
            }
            else if(display->row[display->page] == 1)
            {
                display->page = Display::Page::Load;
                processor.scanPresetDir();
                display->loadMenu(&processor.presets);          
            }
            else if(display->row[display->page] == 2)
            {
                // INIT 
            }
        }
        else if(display->page == Display::Page::Save)
        {
            auto presetName = display->inputBox.getText();
            if(!presetName.isEmpty()) 
            {
                processor.savePreset(presetName, false);
                display->page = Display::Page::MainMenu;
                display->mainMenu();
            }
        }
        else 
        {
            auto control = core::grid.control(display->uid);
            setOption(display->uid, -control->step, control->max);
            display->switchPage(&processor, display->page);
        }
    };

   /***************************************************************************************************************************
    * 
    *  λ - Soft G (Step Right) (Scale Down for Scope)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::sg }))]->onClick = [this]
    {
        if(display->page == Display::Page::CroA)
        {
            auto uid = core::uid_t { core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::scale };
            auto control = core::grid.control(uid);
            setOption(uid, -control->step, control->max);
        }
        else if(display->page == Display::Page::MainMenu)
        {
        }
        else 
        {
            auto control = core::grid.control(display->uid);
            setOption(display->uid, control->step, control->max);
            display->switchPage(&processor, display->page);
        }
    };

   /***************************************************************************************************************************
    * 
    *  λ - Soft H (Jump Right) (Scale Up for Scope)
    * 
    **************************************************************************************************************************/   
    button[(core::grid.getIndex(core::uid_t{ core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::sh }))]->onClick = [this]
    {
        if(display->page == Display::Page::CroA)
        {
            auto uid = core::uid_t { core::map::module::cro, 0, core::map::cv::c, core::cro::ctl::scale };
            auto control = core::grid.control(uid);
            setOption(uid, control->step, control->max);
        }
        else if(display->page == Display::Page::MainMenu)
        {
        }
        else 
        {
            auto control = core::grid.control(display->uid);
            setOption(display->uid, control->max, control->max);
            display->switchPage(&processor, display->page);
        }
    };


    setResizable(false, false);
    setSize (core::constraints::W, core::constraints::H);
    setWantsKeyboardFocus (true);

    processor.suspendProcessing(false);

    startTimerHz(core::settings::scope_fps);
    std::cout<<"-- Editor created\n";

} 

void Editor::setOption(const core::uid_t& uid, const float delta, const float max)
{
    auto index = core::grid.getIndex(uid);

    processor.parameters[index] = processor.tree.getParameter(core::grid.name(uid, true));
    float value = processor.parameters[index]->convertFrom0to1(processor.parameters[index]->getValue());
    value += delta;
    if      (value > max) value = max;
    else if (value < 0.0f) value = 0.0f;
    processor.parameters[index]->beginChangeGesture();
    processor.parameters[index]->setValueNotifyingHost(processor.parameters[index]->convertTo0to1(value));
    processor.parameters[index]->endChangeGesture();
}



/*****************************************************************************************************************************
* 
* Modmatrix routines
* 
******************************************************************************************************************************/

void Editor::loadMatrix()
{
    int i = 0;
    for(int x = 0; x < core::grid.count(core::Control::input); ++x)
    {
        for(int y = 0; y < core::grid.count(core::Control::output); ++y)
        {
            processor.matrix[i] = processor.tree.getParameter("mm" + juce::String(i)); 
            processor.spiro.bay->matrix.set(x, y, core::bool_from_range(processor.matrix[i]->getValue()));
            ++i;
        }
    }
    processor.sockets->load();
    processor.sockets->repaint();
}

void Editor::saveMatrix()
{
    int i = 0;
    for(int x = 0; x < core::grid.count(core::Control::input); ++x)
    {
        for(int y = 0; y < core::grid.count(core::Control::output); ++y)
        {
            processor.matrix[i] = processor.tree.getParameter("mm" + juce::String(i)); 
            processor.matrix[i]->beginChangeGesture();
            processor.matrix[i]->setValueNotifyingHost(processor.spiro.bay->matrix.get(x, y) ? 1.0f : 0.0f);
            processor.matrix[i]->endChangeGesture();
            ++i;
        }
    }
}

void Editor::clearMatrix()
{
    int i = 0;
    for(int x = 0; x < core::grid.count(core::Control::input); ++x)
    {
        for(int y = 0; y < core::grid.count(core::Control::output); ++y)
        {
            processor.spiro.bay->matrix.set(x, y, false);
            ++i;
        }
    }
    processor.sockets->load();
    processor.sockets->repaint();
}


void Editor::loadCall() 
{ 
    loadMatrix();
}

void Editor::saveCall() 
{ 
    saveMatrix();
}

void Editor::resetCall()
{
    display = std::make_unique<Display>(&processor, processor.buffer, core::constraints::oled);
    display->addListener(this);
    display->scope_scale = processor.spiro.rack.at(core::map::module::cro, 0)->ccv[core::cro::ctl::scale];
    display->scope_type  = processor.spiro.rack.at(core::map::module::cro, 0)->ccv[core::cro::ctl::type];
    
    display->setBounds
    (
       core::constraints::oled.x,
       core::constraints::oled.y,
       core::constraints::oled.w,
       core::constraints::oled.h
    );
    display->setOpaque(true); // MUST
	display->setPaintingIsUnclipped(true);

    addAndMakeVisible(display.get());
}

void Editor::switchEnvelope(const int e)
{
    auto uid    = core::uid_t { core::map::module::env, e, core::map::cv::c, core::env::ctl::select };
    auto name   = core::grid.name(uid, true);
    auto value  = processor.tree.getRawParameterValue(name); 
    std::cout<<*value<<"\n";
    env[e].get()->setVisible(*value);
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
    static int f = 0;
    if(fade) ++f;
    if(f > core::settings::scope_fps) { fade = false; f = 0; stopTimer(); };
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

    for(int i = 0; i < envn; ++i)
    {
        juce::Rectangle<int> r 
        {
            core::constraints::envd.x,
            core::constraints::envd.y,
            core::constraints::envd.w,
            core::constraints::envd.h 
        };
        env[i].get()->setBounds(r);
        switchEnvelope(i);
    }

    // for(int i = 0; i < envn; ++i)
    // {
    //     auto uid = core::uid_t { core::map::module::env, i, core::map::cv::c, core::env::ctl::select };
    //     auto name = core::grid.name(uid, true);
    //     auto value = processor.tree.getRawParameterValue(name); 
    //     std::cout<<*value<<"\n";
    //     env[i].get()->setVisible(*value);
    // }

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
    loadMatrix();

    std::cout<<"-- Editor resized\n";
}
