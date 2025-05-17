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

#include "PluginProcessor.h"
#include "PluginEditor.h"


SpiroSynthEditor::SpiroSynthEditor(SpiroSynth& p, juce::AudioProcessorValueTreeState& vts): AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
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

    display = std::make_unique<Display>(audioProcessor.c_buffer.get(), mwc.lcd_display.x, mwc.lcd_display.y, mwc.lcd_display.w, mwc.lcd_display.h);
    pot = std::make_unique<SpriteSlider[]>(core::settings::pot_n);

    for(int i = 0; i < core::settings::pot_n; ++i)
    {
        interface::potentiometer_list p = static_cast<interface::potentiometer_list>(i);

        pot[i].setPaintingIsUnclipped(true);
        pot[i].init(pot_sprite[(int)slider_list.at(p).type].get(), slider_list.at(p).type == slider_descriptor::type_t::Encoder);
        pot_attachment[i].reset (new SliderAttachment(valueTreeState, slider_list.at(p).id, pot[i]));
        addAndMakeVisible (pot[i]);
    }

    for(int i = 0; i < core::settings::btn_n; ++i)
    {
        interface::button_list p = static_cast<interface::button_list>(i);
        auto bt = std::make_unique<juce::ImageButton>(button_list.at(p).name);
        button.emplace_back (std::move(bt));

        button.at(i).get()->setPaintingIsUnclipped(true);
        button.at(i).get()->setClickingTogglesState(button_list.at(p).toggle);
        button.at(i).get()->setRadioGroupId(button_list.at(p).radio_id, juce::dontSendNotification);
        button.at(i).get()->setImages (false, false, true, *btn_sprite[(int)button_list.at(p).type][2], 1.0f, {}, 
                                                 *btn_sprite[(int)button_list.at(p).type][1], 1.0f, {}, 
                                                 *btn_sprite[(int)button_list.at(p).type][0], 1.0f, {});

        addAndMakeVisible(button.at(i).get());
    }

    for(int i = 0; i < core::settings::env_n; ++i)
    {
        env[i].id = i;
        env[i].addListener(this);
        env[i].setOpaque(true);
        env[i].setPaintingIsUnclipped(true);
        addAndMakeVisible(env[i]);
    }
    audioProcessor.addListener(this);
    display->addListener(this);

    display->setOpaque(true); // MUST
	display->setPaintingIsUnclipped(true);
    addAndMakeVisible(display.get());
    addAndMakeVisible(audioProcessor.sockets.get());

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - VCOs
    * 
    * 
    **************************************************************************************************************************/

    button.at(interface::button_list::vco_a).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::vco_a;
        display->VCOMenu(&audioProcessor.spiro.vco[0], 0);
    };

    button.at(interface::button_list::vco_b).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::vco_b;
        display->VCOMenu(&audioProcessor.spiro.vco[1], 1);
    };

    button.at(interface::button_list::vco_c).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::vco_c;
        display->VCOMenu(&audioProcessor.spiro.vco[2], 2);
    };

    button.at(interface::button_list::vco_d).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::vco_d;
        display->VCOMenu(&audioProcessor.spiro.vco[3], 3);
    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Dynamic systems
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::chs_a).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::chs_a;
        display->ChaosMenu(&audioProcessor.spiro.chs[0], 0);
    };

    button.at(interface::button_list::chs_b).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::chs_b;
        display->ChaosMenu(&audioProcessor.spiro.chs[1], 1);
    };

    /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - LFOs
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::lfo_a).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::lfo_a;
        display->LFOMenu(&audioProcessor.spiro.lfo[0], 0);
    };

    button.at(interface::button_list::lfo_b).get()->onClick = [this]
    {
        stopTimer();
        display->page = Display::page_t::lfo_b;
        display->LFOMenu(&audioProcessor.spiro.lfo[1], 1);
    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft A (Cancel of Rewind)
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_a).get()->onClick = [this]
    {
        float v;
        switch(display->page)
        {
            case Display::page_t::scope: scopeOptions(0, false); break;
            case Display::page_t::chs_a: chsOptions(0, false); break;
            case Display::page_t::chs_b: chsOptions(1, false); break;
            case Display::page_t::lfo_a: lfoOptions(0, false); break;
            case Display::page_t::lfo_b: lfoOptions(1, false); break;
            case Display::page_t::vco_a: vcoOptions(0, false); break;
            case Display::page_t::vco_b: vcoOptions(1, false); break;
            case Display::page_t::vco_c: vcoOptions(2, false); break;
            case Display::page_t::vco_d: vcoOptions(3, false); break;
            case Display::page_t::envs:  envOptions(0.1f, false); break;
            case Display::page_t::load:          
                stopTimer();
                display->MainMenu();
                break;

            case Display::page_t::save:          
                stopTimer();
                display->MainMenu();
                break;

            case Display::page_t::menu:          
                startTimerHz(core::settings::scope_fps);
                display->page = Display::page_t::scope;
                display->layer_on = false;
                button.at(interface::button_list::scope).get()->setToggleState(true, juce::NotificationType::dontSendNotification);
                break;

            default:
                break;
        }
        display->repaint();

    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft B (OK or Step Back)
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_b).get()->onClick = [this]
    {
        float v;
        switch(display->page)
        {
            case Display::page_t::scope: scopeOptions(0, true); break;
            case Display::page_t::chs_a: chsOptions(0, false); break;
            case Display::page_t::chs_b: chsOptions(1, false); break;
            case Display::page_t::lfo_a: lfoOptions(0, false); break;
            case Display::page_t::lfo_b: lfoOptions(1, false); break;
            case Display::page_t::vco_a: vcoOptions(0, false); break;
            case Display::page_t::vco_b: vcoOptions(1, false); break;
            case Display::page_t::vco_c: vcoOptions(2, false); break;
            case Display::page_t::vco_d: vcoOptions(3, false); break;
            case Display::page_t::envs:  envOptions(0.01f, false); break;

            case Display::page_t::save:
            {
                juce::String presetName = display->input_box.getText();
                if (presetName.isEmpty()) 
                {
                    break;
                }
                audioProcessor.savePreset (presetName, false);
                display->MainMenu();
            }
            break;

            case Display::page_t::load:
            {
                audioProcessor.loadPreset(audioProcessor.presets.at(display->row + display->load_page * display->rows_max).first);
                loadMatrix();
                envLoad();
                audioProcessor.scanPresetDir();
                display->MainMenu();
            }
            break;

            case Display::page_t::menu:
            {
                if(display->row == 0)
                {
                    startTimerHz(core::settings::scope_fps);
                    display->page = Display::page_t::save;
                    display->SaveMenu();
                    display->grabKeyboardFocus();
                    display->input_box.setText(audioProcessor.currentPresetName);
                }
                else if(display->row == 1)
                {
                    display->page = Display::page_t::load;
                    audioProcessor.scanPresetDir();
                    display->LoadMenu(&audioProcessor.presets);
                }
                else if(display->row == 2)
                {
                   clearMatrix();
                   audioProcessor.reset();
                }
            }
            break;


            default:
                break;
        }
        display->repaint();
    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft C (Step back or forward)
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_c).get()->onClick = [this]
    {
        switch(display->page)
        {
            case Display::page_t::scope: scopeOptions(1, false); break;
            case Display::page_t::chs_a: chsOptions(0, true); break;
            case Display::page_t::chs_b: chsOptions(1, true); break;
            case Display::page_t::lfo_a: lfoOptions(0, true); break;
            case Display::page_t::lfo_b: lfoOptions(1, true); break;
            case Display::page_t::vco_a: vcoOptions(0, true); break;
            case Display::page_t::vco_b: vcoOptions(1, true); break;
            case Display::page_t::vco_c: vcoOptions(2, true); break;
            case Display::page_t::vco_d: vcoOptions(3, true); break;

            case Display::page_t::load:
                display->load_page--;
                display->LoadMenu(&audioProcessor.presets);
                break;

            case Display::page_t::envs:  envOptions(0.01f, true); break;

            default: break;
        }
        display->repaint();
    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft D
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_d).get()->onClick = [this]
    {
        switch(display->page)
        {
            case Display::page_t::scope: scopeOptions(1, true); break;
            case Display::page_t::chs_a: chsOptions(0, true); break;
            case Display::page_t::chs_b: chsOptions(1, true); break;
            case Display::page_t::lfo_a: lfoOptions(0, true); break;
            case Display::page_t::lfo_b: lfoOptions(1, true); break;
            case Display::page_t::vco_a: vcoOptions(0, true); break;
            case Display::page_t::vco_b: vcoOptions(1, true); break;
            case Display::page_t::vco_c: vcoOptions(2, true); break;
            case Display::page_t::vco_d: vcoOptions(3, true); break;
                
            case Display::page_t::load:
                display->load_page++;
                display->LoadMenu(&audioProcessor.presets);
                break;

            case Display::page_t::envs:  envOptions(0.1f, true); break;

            default: break;
        }
        display->repaint();
    };


   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft E
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_e).get()->onClick = [this]
    {
        switch(display->page)
        {
            case Display::page_t::vco_a:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[0], 0);
                break;

            case Display::page_t::vco_b:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[1], 1);
                break;

            case Display::page_t::vco_c:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[2], 2);
                break;

            case Display::page_t::vco_d:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[3], 3);
                break;

            case Display::page_t::load:
                display->row--;
                display->LoadMenu(&audioProcessor.presets);
                break;

            case Display::page_t::envs:
                display->row = 0;
                display->EnvelopeMenu(&audioProcessor.spiro.env[envelope_shown], envelope_shown);
                break;

            case Display::page_t::menu:
                display->row--;
                display->MainMenu();
                break;

            default: break;
        }
        display->repaint();
    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft F ( Step Up )
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_f).get()->onClick = [this]
    {
        switch(display->page)
        {
            case Display::page_t::vco_a:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[0], 0);
                break;

            case Display::page_t::vco_b:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[1], 1);
                break;

            case Display::page_t::vco_c:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[2], 2);
                break;

            case Display::page_t::vco_d:
                display->row--;
                display->VCOMenu(&audioProcessor.spiro.vco[3], 3);
                break;

            case Display::page_t::load:
                display->row--;
                display->LoadMenu(&audioProcessor.presets);
                break;

            case Display::page_t::envs:
                display->row--;
                display->EnvelopeMenu(&audioProcessor.spiro.env[envelope_shown], envelope_shown);
                break;

            case Display::page_t::menu:
                display->row--;
                display->MainMenu();
                break;

            default: break;
        }
        display->repaint();
    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft G ( Step Down )
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_g).get()->onClick = [this]
    {
        switch(display->page)
        {
            case Display::page_t::vco_a:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[0], 0);
                break;

            case Display::page_t::vco_b:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[1], 1);
                break;

            case Display::page_t::vco_c:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[2], 2);
                break;

            case Display::page_t::vco_d:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[3], 3);
                break;

            case Display::page_t::load:
                display->row++;
                display->LoadMenu(&audioProcessor.presets);
                break;

            case Display::page_t::envs:
                display->row++;
                display->EnvelopeMenu(&audioProcessor.spiro.env[envelope_shown], envelope_shown);
                break;

            case Display::page_t::menu:
                display->row++;
                display->MainMenu();
                break;

            default: break;
        }
        display->repaint();
    };

   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Soft H
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::soft_h).get()->onClick = [this]
    {
        switch(display->page)
        {
            case Display::page_t::vco_a:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[0], 0);
                break;

            case Display::page_t::vco_b:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[1], 1);
                break;

            case Display::page_t::vco_c:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[2], 2);
                break;

            case Display::page_t::vco_d:
                display->row++;
                display->VCOMenu(&audioProcessor.spiro.vco[3], 3);
                break;

            case Display::page_t::load:
                display->row++;
                display->LoadMenu(&audioProcessor.presets);
                break;

            case Display::page_t::envs:
                display->row++;
                display->EnvelopeMenu(&audioProcessor.spiro.env[envelope_shown], envelope_shown);
                break;

            case Display::page_t::menu:
                display->row++;
                display->MainMenu();
                break;

            default: break;
        }
        display->repaint();
    };

    /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Menu Button
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::menu).get()->onClick = [this]
    {
        stopTimer();
        display->MainMenu();
    };
    /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Scope Button
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::scope).get()->onClick = [this]
    {
        startTimerHz(core::settings::scope_fps);
        display->page = Display::page_t::scope;
        display->layer_on = false;
    };


   /***************************************************************************************************************************
    * 
    * 
    *  LAMBDA - Envelope switches
    * 
    * 
    **************************************************************************************************************************/
    button.at(interface::button_list::env_a).get()->onClick = [this] { envSwitch(0); };
    button.at(interface::button_list::env_b).get()->onClick = [this] { envSwitch(1); };
    button.at(interface::button_list::env_c).get()->onClick = [this] { envSwitch(2); };
    button.at(interface::button_list::env_d).get()->onClick = [this] { envSwitch(3); };

    button.at(interface::button_list::envs).get()->onClick  = [this]
    {
        display->page = Display::page_t::envs;
        display->EnvelopeMenu(&audioProcessor.spiro.env[envelope_shown], envelope_shown);
    };

    setResizable(false, false);
    setSize (mwc.width, mwc.height);
    setWantsKeyboardFocus (true);
} 
/* EOF Constructor ***********************************************************************************************************/

/*****************************************************************************************************************************
* 
* 
* Load parameters 
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::onReload()
{
    display.get()->scope_scale = audioProcessor.tree.getRawParameterValue(parameter_list.at(interface::parameter_list::scope_scale).id);
    display.get()->scope_type =  audioProcessor.tree.getRawParameterValue(parameter_list.at(interface::parameter_list::scope_type).id);
}

/*****************************************************************************************************************************
* 
* 
* VCO Options
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::vcoOptions(int id, bool plus)
{
    float v;
    interface::parameter_list t_param, o_param, f_param;
    switch(id)
    {
        case 0: 
            t_param = interface::parameter_list::vco_a_freerun; 
            o_param = interface::parameter_list::vco_a_oct;
            f_param = interface::parameter_list::vco_a_form;
            break;
        case 1: 
            t_param = interface::parameter_list::vco_b_freerun;
            o_param = interface::parameter_list::vco_b_oct; 
            f_param = interface::parameter_list::vco_b_form;
            break;
        case 2: 
            t_param = interface::parameter_list::vco_c_freerun; 
            o_param = interface::parameter_list::vco_c_oct; 
            f_param = interface::parameter_list::vco_c_form;
            break;
        case 3: 
            t_param = interface::parameter_list::vco_d_freerun; 
            o_param = interface::parameter_list::vco_d_oct; 
            f_param = interface::parameter_list::vco_d_form;
            break;
        default: 
            break;
    };

    if(display->row == 0)
    {
        audioProcessor.parameter[f_param] = audioProcessor.tree.getParameter (parameter_list.at(f_param).id);
        v = audioProcessor.parameter[f_param]->convertFrom0to1(audioProcessor.parameter[f_param]->getValue());
        if(plus)
        {
            v += 1.0f;
            if(v > (2.0f)) v = 0.0f;
        }
        else
        {
            v -= 1.0f;
            if(v < 0.0f) v = (2.0f);
        }

        audioProcessor.parameter[f_param]->beginChangeGesture();
        audioProcessor.parameter[f_param]->setValueNotifyingHost(audioProcessor.parameter[f_param]->convertTo0to1(v));
        audioProcessor.parameter[f_param]->endChangeGesture();          
    }
    else if(display->row == 1)
    {
        audioProcessor.parameter[t_param] = audioProcessor.tree.getParameter (parameter_list.at(t_param).id);
        v = audioProcessor.parameter[t_param]->getValue();
        v > 0.5f ? v = 0.0f : v = 1.0f;
        audioProcessor.parameter[t_param]->beginChangeGesture();
        audioProcessor.parameter[t_param]->setValueNotifyingHost(v);
        audioProcessor.parameter[t_param]->endChangeGesture();
    }
    else if(display->row == 2)
    {
        audioProcessor.parameter[o_param] = audioProcessor.tree.getParameter (parameter_list.at(o_param).id);
        v = audioProcessor.parameter[o_param]->getValue();
        if(plus) v += 0.1f;
        else     v -= 0.1f;
        if      (v > 0.6f) v = 0.6f;
        else if (v < 0.0f) v = 0.0f;

        audioProcessor.parameter[o_param]->beginChangeGesture();
        audioProcessor.parameter[o_param]->setValueNotifyingHost(v);
        audioProcessor.parameter[o_param]->endChangeGesture();
    }
    display->VCOMenu(&audioProcessor.spiro.vco[id], id);
}
/*****************************************************************************************************************************
* 
* 
* Dynamic System Options
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::chsOptions(int id, bool plus)
{

    float v;
    interface::parameter_list f_param;
    switch(id)
    {
        case 0: f_param = interface::parameter_list::chs_a_form; break;
        case 1: f_param = interface::parameter_list::chs_b_form; break;
        default: break;
    };

    audioProcessor.parameter[f_param] = audioProcessor.tree.getParameter (parameter_list.at(f_param).id);
    v = audioProcessor.parameter[f_param]->convertFrom0to1(audioProcessor.parameter[f_param]->getValue());
    if(plus)
    {
        v += 1.0f;
        if(v > ((float)n_forms_chaotic - 1.0f)) v = 0.0f;
    }
    else
    {
        v -= 1.0f;
        if(v < 0.0f) v = ((float)n_forms_chaotic - 1.0f);
    }

    audioProcessor.parameter[f_param]->beginChangeGesture();
    audioProcessor.parameter[f_param]->setValueNotifyingHost(audioProcessor.parameter[f_param]->convertTo0to1(v));
    audioProcessor.parameter[f_param]->endChangeGesture();  

    audioProcessor.spiro.chs[id].switch_wave(0);
    display->ChaosMenu(&audioProcessor.spiro.chs[id], id);
}

/*****************************************************************************************************************************
* 
* 
* LFOs Options
* 
* 
******************************************************************************************************************************/

void SpiroSynthEditor::lfoOptions(int id, bool plus)
{

    float v;
    interface::parameter_list f_param;
    switch(id)
    {
        case 0: f_param = interface::parameter_list::lfo_a_form; break;
        case 1: f_param = interface::parameter_list::lfo_b_form; break;
        default: break;
    };

    audioProcessor.parameter[f_param] = audioProcessor.tree.getParameter (parameter_list.at(f_param).id);
    v = audioProcessor.parameter[f_param]->convertFrom0to1(audioProcessor.parameter[f_param]->getValue());
    if(plus)
    {
        v += 1.0f;
        if(v > 4.0f) v = 0.0f;
    }
    else
    {
        v -= 1.0f;
        if(v < 0.0f) v = 4.0f;
    }

    audioProcessor.parameter[f_param]->beginChangeGesture();
    audioProcessor.parameter[f_param]->setValueNotifyingHost(audioProcessor.parameter[f_param]->convertTo0to1(v));
    audioProcessor.parameter[f_param]->endChangeGesture();  

    display->LFOMenu(&audioProcessor.spiro.lfo[id], id);
}


/*****************************************************************************************************************************
* 
* 
* Scope Options
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::scopeOptions(int id, bool plus)
{
    float v;
    interface::parameter_list f_param;
    if(id == 0)
    {
        f_param = interface::parameter_list::scope_type;

        audioProcessor.parameter[f_param] = audioProcessor.tree.getParameter (parameter_list.at(f_param).id);
        v = audioProcessor.parameter[f_param]->convertFrom0to1(audioProcessor.parameter[f_param]->getValue());
        if(plus)
        {
            v += 1.0f;
            if(v > 1.0f) v = 0.0f;
        }
        else
        {
            v -= 1.0f;
            if(v < 0.0f) v = 1.0f;
        }

        audioProcessor.parameter[f_param]->beginChangeGesture();
        audioProcessor.parameter[f_param]->setValueNotifyingHost(audioProcessor.parameter[f_param]->convertTo0to1(v));
        audioProcessor.parameter[f_param]->endChangeGesture();
    }
    else if(id == 1)
    {
        f_param = interface::parameter_list::scope_scale;

        audioProcessor.parameter[f_param] = audioProcessor.tree.getParameter (parameter_list.at(f_param).id);
        v = audioProcessor.parameter[f_param]->convertFrom0to1(audioProcessor.parameter[f_param]->getValue());
        if(plus)
        {
            v += 0.1f;
            if(v > 1.0f) v = 1.0f;
        }
        else
        {
            v -= 0.1f;
            if(v < 0.01f) v = 0.01f;
        }

        audioProcessor.parameter[f_param]->beginChangeGesture();
        audioProcessor.parameter[f_param]->setValueNotifyingHost(audioProcessor.parameter[f_param]->convertTo0to1(v));
        audioProcessor.parameter[f_param]->endChangeGesture();
    }
}
/*****************************************************************************************************************************
* 
* 
* Envelope Switch
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::envSwitch(int id)
{
    env[envelope_shown].setVisible(false);
    env[id].setVisible(true);

    if(button.at(interface::button_list::envs).get()->getToggleState()) 
        display->EnvelopeMenu(&audioProcessor.spiro.env[id], id);

    envelope_shown = id;
}


void SpiroSynthEditor::envOptions(float value, bool plus)
{
    interface::parameter_list a;
    switch (envelope_shown)
    {
        
        case 0: a = interface::parameter_list::env_a_scale; break;
        case 1: a = interface::parameter_list::env_b_scale; break;
        case 2: a = interface::parameter_list::env_c_scale; break;
        case 3: a = interface::parameter_list::env_d_scale; break;
        default: break;
    };

    audioProcessor.parameter[a] = audioProcessor.tree.getParameter (parameter_list.at(a).id);
    float v = audioProcessor.parameter[a]->getValue();

    if(plus)
    {
        if (v < 1.00f) v += value;
        else v = 1.0f;
    }
    else
    {
        if (v > 0.02f) v -= value;
        else v = 0.01f;
    }
    
    audioProcessor.parameter[a]->beginChangeGesture();
    audioProcessor.parameter[a]->setValueNotifyingHost(v);
    audioProcessor.parameter[a]->endChangeGesture();
    
    display->EnvelopeMenu(&audioProcessor.spiro.env[envelope_shown], envelope_shown);
}

/*****************************************************************************************************************************
* 
* 
* Envelope Broadcast
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::envBroadcast(int id)
{
    interface::parameter_list env_form_a;
    interface::parameter_list env_form_h;
    interface::parameter_list env_form_d;
    interface::parameter_list env_form_s;
    interface::parameter_list env_form_r;

    interface::parameter_list env_time_a;
    interface::parameter_list env_time_h;
    interface::parameter_list env_time_d;
    interface::parameter_list env_time_s;
    interface::parameter_list env_time_r;

    interface::parameter_list env_value_a;
    interface::parameter_list env_value_h;
    interface::parameter_list env_value_d;
    interface::parameter_list env_value_s;
    interface::parameter_list env_value_r;
    switch(id)
    {
        case 0: 
            env_form_a = interface::parameter_list::env_a_form_a; 
            env_form_h = interface::parameter_list::env_a_form_h; 
            env_form_d = interface::parameter_list::env_a_form_d; 
            env_form_s = interface::parameter_list::env_a_form_s; 
            env_form_r = interface::parameter_list::env_a_form_r;

            env_time_a = interface::parameter_list::env_a_time_a;
            env_time_h = interface::parameter_list::env_a_time_h;
            env_time_d = interface::parameter_list::env_a_time_d;
            env_time_s = interface::parameter_list::env_a_time_s;
            env_time_r = interface::parameter_list::env_a_time_r;

            env_value_a = interface::parameter_list::env_a_value_a;
            env_value_h = interface::parameter_list::env_a_value_h;
            env_value_d = interface::parameter_list::env_a_value_d;
            env_value_s = interface::parameter_list::env_a_value_s;
            env_value_r = interface::parameter_list::env_a_value_r;

            break;

        case 1: 
            env_form_a = interface::parameter_list::env_b_form_a; 
            env_form_h = interface::parameter_list::env_b_form_h; 
            env_form_d = interface::parameter_list::env_b_form_d; 
            env_form_s = interface::parameter_list::env_b_form_s; 
            env_form_r = interface::parameter_list::env_b_form_r;

            env_time_a = interface::parameter_list::env_b_time_a;
            env_time_h = interface::parameter_list::env_b_time_h;
            env_time_d = interface::parameter_list::env_b_time_d;
            env_time_s = interface::parameter_list::env_b_time_s;
            env_time_r = interface::parameter_list::env_b_time_r;

            env_value_a = interface::parameter_list::env_b_value_a;
            env_value_h = interface::parameter_list::env_b_value_h;
            env_value_d = interface::parameter_list::env_b_value_d;
            env_value_s = interface::parameter_list::env_b_value_s;
            env_value_r = interface::parameter_list::env_b_value_r;

            break;

        case 2: 
            env_form_a = interface::parameter_list::env_c_form_a; 
            env_form_h = interface::parameter_list::env_c_form_h; 
            env_form_d = interface::parameter_list::env_c_form_d; 
            env_form_s = interface::parameter_list::env_c_form_s; 
            env_form_r = interface::parameter_list::env_c_form_r;

            env_time_a = interface::parameter_list::env_c_time_a;
            env_time_h = interface::parameter_list::env_c_time_h;
            env_time_d = interface::parameter_list::env_c_time_d;
            env_time_s = interface::parameter_list::env_c_time_s;
            env_time_r = interface::parameter_list::env_c_time_r;

            env_value_a = interface::parameter_list::env_c_value_a;
            env_value_h = interface::parameter_list::env_c_value_h;
            env_value_d = interface::parameter_list::env_c_value_d;
            env_value_s = interface::parameter_list::env_c_value_s;
            env_value_r = interface::parameter_list::env_c_value_r;

            break;

        case 3: 
            env_form_a = interface::parameter_list::env_d_form_a; 
            env_form_h = interface::parameter_list::env_d_form_h; 
            env_form_d = interface::parameter_list::env_d_form_d; 
            env_form_s = interface::parameter_list::env_d_form_s; 
            env_form_r = interface::parameter_list::env_d_form_r;

            env_time_a = interface::parameter_list::env_d_time_a;
            env_time_h = interface::parameter_list::env_d_time_h;
            env_time_d = interface::parameter_list::env_d_time_d;
            env_time_s = interface::parameter_list::env_d_time_s;
            env_time_r = interface::parameter_list::env_d_time_r;

            env_value_a = interface::parameter_list::env_d_value_a;
            env_value_h = interface::parameter_list::env_d_value_h;
            env_value_d = interface::parameter_list::env_d_value_d;
            env_value_s = interface::parameter_list::env_d_value_s;
            env_value_r = interface::parameter_list::env_d_value_r;

            break;

        default: 
            break;
    };

    audioProcessor.parameter[env_form_a] = audioProcessor.tree.getParameter (parameter_list.at(env_form_a).id);
    audioProcessor.parameter[env_form_a]->beginChangeGesture();
    audioProcessor.parameter[env_form_a]->setValueNotifyingHost(audioProcessor.parameter[env_form_a]->convertTo0to1(env[id].node[1].curve));
    audioProcessor.parameter[env_form_a]->endChangeGesture();  

    audioProcessor.parameter[env_form_h] = audioProcessor.tree.getParameter (parameter_list.at(env_form_h).id);
    audioProcessor.parameter[env_form_h]->beginChangeGesture();
    audioProcessor.parameter[env_form_h]->setValueNotifyingHost(audioProcessor.parameter[env_form_h]->convertTo0to1(env[id].node[2].curve));
    audioProcessor.parameter[env_form_h]->endChangeGesture();  

    audioProcessor.parameter[env_form_d] = audioProcessor.tree.getParameter (parameter_list.at(env_form_d).id);
    audioProcessor.parameter[env_form_d]->beginChangeGesture();
    audioProcessor.parameter[env_form_d]->setValueNotifyingHost(audioProcessor.parameter[env_form_d]->convertTo0to1(env[id].node[3].curve));
    audioProcessor.parameter[env_form_d]->endChangeGesture();

    audioProcessor.parameter[env_form_s] = audioProcessor.tree.getParameter (parameter_list.at(env_form_s).id);
    audioProcessor.parameter[env_form_s]->beginChangeGesture();
    audioProcessor.parameter[env_form_s]->setValueNotifyingHost(audioProcessor.parameter[env_form_s]->convertTo0to1(env[id].node[4].curve));
    audioProcessor.parameter[env_form_s]->endChangeGesture();

    audioProcessor.parameter[env_form_r] = audioProcessor.tree.getParameter (parameter_list.at(env_form_r).id);
    audioProcessor.parameter[env_form_r]->beginChangeGesture();
    audioProcessor.parameter[env_form_r]->setValueNotifyingHost(audioProcessor.parameter[env_form_r]->convertTo0to1(env[id].node[5].curve));
    audioProcessor.parameter[env_form_r]->endChangeGesture();


    audioProcessor.parameter[env_time_a] = audioProcessor.tree.getParameter (parameter_list.at(env_time_a).id);
    audioProcessor.parameter[env_time_a]->beginChangeGesture();
    audioProcessor.parameter[env_time_a]->setValueNotifyingHost(audioProcessor.parameter[env_time_a]->convertTo0to1(env[id].node[1].time));
    audioProcessor.parameter[env_time_a]->endChangeGesture();

    audioProcessor.parameter[env_time_h] = audioProcessor.tree.getParameter (parameter_list.at(env_time_h).id);
    audioProcessor.parameter[env_time_h]->beginChangeGesture();
    audioProcessor.parameter[env_time_h]->setValueNotifyingHost(audioProcessor.parameter[env_time_h]->convertTo0to1(env[id].node[2].time));
    audioProcessor.parameter[env_time_h]->endChangeGesture();

    audioProcessor.parameter[env_time_d] = audioProcessor.tree.getParameter (parameter_list.at(env_time_d).id);
    audioProcessor.parameter[env_time_d]->beginChangeGesture();
    audioProcessor.parameter[env_time_d]->setValueNotifyingHost(audioProcessor.parameter[env_time_d]->convertTo0to1(env[id].node[3].time));
    audioProcessor.parameter[env_time_d]->endChangeGesture();

    audioProcessor.parameter[env_time_s] = audioProcessor.tree.getParameter (parameter_list.at(env_time_s).id);
    audioProcessor.parameter[env_time_s]->beginChangeGesture();
    audioProcessor.parameter[env_time_s]->setValueNotifyingHost(audioProcessor.parameter[env_time_s]->convertTo0to1(env[id].node[4].time));
    audioProcessor.parameter[env_time_s]->endChangeGesture();

    audioProcessor.parameter[env_time_r] = audioProcessor.tree.getParameter (parameter_list.at(env_time_r).id);
    audioProcessor.parameter[env_time_r]->beginChangeGesture();
    audioProcessor.parameter[env_time_r]->setValueNotifyingHost(audioProcessor.parameter[env_time_r]->convertTo0to1(env[id].node[5].time));
    audioProcessor.parameter[env_time_r]->endChangeGesture();

    audioProcessor.parameter[env_value_a] = audioProcessor.tree.getParameter (parameter_list.at(env_value_a).id);
    audioProcessor.parameter[env_value_a]->beginChangeGesture();
    audioProcessor.parameter[env_value_a]->setValueNotifyingHost(audioProcessor.parameter[env_value_a]->convertTo0to1(env[id].node[1].value));
    audioProcessor.parameter[env_value_a]->endChangeGesture();

    audioProcessor.parameter[env_value_h] = audioProcessor.tree.getParameter (parameter_list.at(env_value_h).id);
    audioProcessor.parameter[env_value_h]->beginChangeGesture();
    audioProcessor.parameter[env_value_h]->setValueNotifyingHost(audioProcessor.parameter[env_value_h]->convertTo0to1(env[id].node[2].value));
    audioProcessor.parameter[env_value_h]->endChangeGesture();

    audioProcessor.parameter[env_value_d] = audioProcessor.tree.getParameter (parameter_list.at(env_value_d).id);
    audioProcessor.parameter[env_value_d]->beginChangeGesture();
    audioProcessor.parameter[env_value_d]->setValueNotifyingHost(audioProcessor.parameter[env_value_d]->convertTo0to1(env[id].node[3].value));
    audioProcessor.parameter[env_value_d]->endChangeGesture();

    audioProcessor.parameter[env_value_s] = audioProcessor.tree.getParameter (parameter_list.at(env_value_s).id);
    audioProcessor.parameter[env_value_s]->beginChangeGesture();
    audioProcessor.parameter[env_value_s]->setValueNotifyingHost(audioProcessor.parameter[env_value_s]->convertTo0to1(env[id].node[4].value));
    audioProcessor.parameter[env_value_s]->endChangeGesture();
    
    audioProcessor.parameter[env_value_r] = audioProcessor.tree.getParameter (parameter_list.at(env_value_r).id);
    audioProcessor.parameter[env_value_r]->beginChangeGesture();
    audioProcessor.parameter[env_value_r]->setValueNotifyingHost(audioProcessor.parameter[env_value_r]->convertTo0to1(env[id].node[5].value));
    audioProcessor.parameter[env_value_r]->endChangeGesture();
}

void SpiroSynthEditor::envLoad() 
{ 
    juce::RangedAudioParameter* p;
    
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_form_a).id);
    env[0].node[1].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_form_h).id);
    env[0].node[2].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_form_d).id);
    env[0].node[3].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_form_s).id);
    env[0].node[4].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_form_r).id);
    env[0].node[5].curve = (p->convertFrom0to1(p->getValue())); 

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_form_a).id);
    env[1].node[1].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_form_h).id);
    env[1].node[2].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_form_d).id);
    env[1].node[3].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_form_s).id);
    env[1].node[4].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_form_r).id);
    env[1].node[5].curve = (p->convertFrom0to1(p->getValue())); 

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_form_a).id);
    env[2].node[1].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_form_h).id);
    env[2].node[2].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_form_d).id);
    env[2].node[3].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_form_s).id);
    env[2].node[4].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_form_r).id);
    env[2].node[5].curve = (p->convertFrom0to1(p->getValue())); 

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_form_a).id);
    env[3].node[1].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_form_h).id);
    env[3].node[2].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_form_d).id);
    env[3].node[3].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_form_s).id);
    env[3].node[4].curve = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_form_r).id);
    env[3].node[5].curve = (p->convertFrom0to1(p->getValue())); 



    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_value_a).id);
    env[0].node[1].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_value_h).id);
    env[0].node[2].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_value_d).id);
    env[0].node[3].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_value_s).id);
    env[0].node[4].value = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_value_r).id);
    env[0].node[5].value = (p->convertFrom0to1(p->getValue())); 

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_value_a).id);
    env[1].node[1].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_value_h).id);
    env[1].node[2].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_value_d).id);
    env[1].node[3].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_value_s).id);
    env[1].node[4].value = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_value_r).id);
    env[1].node[5].value = (p->convertFrom0to1(p->getValue()));

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_value_a).id);
    env[2].node[1].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_value_h).id);
    env[2].node[2].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_value_d).id);
    env[2].node[3].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_value_s).id);
    env[2].node[4].value = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_value_r).id);
    env[2].node[5].value = (p->convertFrom0to1(p->getValue()));

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_value_a).id);
    env[3].node[1].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_value_h).id);
    env[3].node[2].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_value_d).id);
    env[3].node[3].value = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_value_s).id);
    env[3].node[4].value = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_value_r).id);
    env[3].node[5].value = (p->convertFrom0to1(p->getValue()));



    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_time_a).id);
    env[0].node[1].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_time_h).id);
    env[0].node[2].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_time_d).id); 
    env[0].node[3].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_time_s).id); 
    env[0].node[4].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_a_time_r).id); 
    env[0].node[5].time = (p->convertFrom0to1(p->getValue()));

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_time_a).id);
    env[1].node[1].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_time_h).id);
    env[1].node[2].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_time_d).id); 
    env[1].node[3].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_time_s).id); 
    env[1].node[4].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_b_time_r).id); 
    env[1].node[5].time = (p->convertFrom0to1(p->getValue()));

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_time_a).id);
    env[2].node[1].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_time_h).id);
    env[2].node[2].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_time_d).id); 
    env[2].node[3].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_time_s).id); 
    env[2].node[4].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_c_time_r).id); 
    env[2].node[5].time = (p->convertFrom0to1(p->getValue()));

    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_time_a).id);
    env[3].node[1].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_time_h).id);
    env[3].node[2].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_time_d).id); 
    env[3].node[3].time = (p->convertFrom0to1(p->getValue())); 
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_time_s).id); 
    env[3].node[4].time = (p->convertFrom0to1(p->getValue()));
    p = audioProcessor.tree.getParameter(parameter_list.at(interface::parameter_list::env_d_time_r).id); 
    env[3].node[5].time = (p->convertFrom0to1(p->getValue()));

    
    for(int i = 0; i < core::settings::env_n; ++i)
    {
        env[i].load();
    }
}


void SpiroSynthEditor::envChanged(int id) { envBroadcast(id); }
void SpiroSynthEditor::bufferDisconnected() 
{
    display.reset();
}

void SpiroSynthEditor::loadCall() 
{ 
    loadMatrix();
    envLoad();

}

void SpiroSynthEditor::saveCall() 
{ 
    saveMatrix();
    for(int i = 0; i < core::settings::env_n; ++i)
    {
        envBroadcast(i);
    }
}
/*****************************************************************************************************************************
* 
* 
* Load/Save Matrix
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::loadMatrix()
{
    int i = 0;
    for(int x = 0; x < core::settings::ports_in; ++x)
    {
        for(int y = 0; y < core::settings::ports_out; ++y)
        {
            audioProcessor.matrix[i] = audioProcessor.tree.getParameter("matrix_" + juce::String(i)); 
            audioProcessor.spiro.bay->matrix.set(x, y, core::bool_from_range(audioProcessor.matrix[i]->getValue()));
            ++i;
        }
    }
    audioProcessor.sockets->load();
    audioProcessor.sockets->repaint();
}

void SpiroSynthEditor::saveMatrix()
{
    int i = 0;
    for(int x = 0; x < core::settings::ports_in; ++x)
    {
        for(int y = 0; y < core::settings::ports_out; ++y)
        {
            audioProcessor.matrix[i] = audioProcessor.tree.getParameter("matrix_" + juce::String(i)); 
            audioProcessor.matrix[i]->beginChangeGesture();
            audioProcessor.matrix[i]->setValueNotifyingHost(audioProcessor.spiro.bay->matrix.get(x, y) ? 1.0f : 0.0f);
            audioProcessor.matrix[i]->endChangeGesture();
            ++i;
        }
    }
}

void SpiroSynthEditor::clearMatrix()
{
    int i = 0;
    for(int x = 0; x < core::settings::ports_in; ++x)
    {
        for(int y = 0; y < core::settings::ports_out; ++y)
        {
            audioProcessor.spiro.bay->matrix.set(x, y, false);
            ++i;
        }
    }
    audioProcessor.sockets->load();
    audioProcessor.sockets->repaint();
}
/*****************************************************************************************************************************
* 
* 
* Destructor
* 
* 
******************************************************************************************************************************/
SpiroSynthEditor::~SpiroSynthEditor()
{
    stopTimer();
    for(int i = 0; i < core::settings::env_n; ++i)
    {
        env[i].removeListener(this);
    }
    audioProcessor.removeListener(this);
    display->removeListener(this);
}


/*****************************************************************************************************************************
* 
* 
*  On Resize
* 
* 
******************************************************************************************************************************/
void SpiroSynthEditor::resized()
{
	bg.setBounds(0, 0, mwc.width, mwc.height);

    for(int i = 0; i < core::settings::env_n; ++i)
    {
        juce::Rectangle<int> r { mwc.env_display.x, mwc.env_display.y, mwc.env_display.w, mwc.env_display.h };
        env[i].setBounds(r);
    }

    for(int i = 0; i < core::settings::btn_n; ++i)
    {
        interface::button_list p = static_cast<interface::button_list>(i);
        juce::Rectangle<int> r { button_list.at(p).pos.x, button_list.at(p).pos.y, button_list.at(p).pos.w, button_list.at(p).pos.h };
        button.at(i).get()->setBounds(r);
    }
   
    for(int i = 0; i < core::settings::pot_n; ++i)
    {
        interface::potentiometer_list p = static_cast<interface::potentiometer_list>(i);
        juce::Rectangle<int> r { slider_list.at(p).pos.x, slider_list.at(p).pos.y, slider_list.at(p).pos.w, slider_list.at(p).pos.h };
        pot[i].setBounds(r);
    }

    display->setBounds(mwc.lcd_display.x, mwc.lcd_display.y, mwc.lcd_display.w, mwc.lcd_display.h);
    startTimerHz(core::settings::scope_fps);

    onReload();

    env[0].setVisible(true);
    env[1].setVisible(false);
    env[2].setVisible(false);
    env[3].setVisible(false);

    audioProcessor.spiro.connect_bus();
    loadMatrix();
    envLoad();
    audioProcessor.suspendProcessing(false);
}
