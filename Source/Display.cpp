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
#include "Display.h"
#include "cso.hpp"
#include "spiro.hpp"

const core::uid_t Display::getUID() const
{
    uint8_t mt;
    switch(page) {
        case VcoA: mt = static_cast<uint8_t>(core::map::module::vco); break;
        case VcoB: mt = static_cast<uint8_t>(core::map::module::vco); break;
        case VcoC: mt = static_cast<uint8_t>(core::map::module::vco); break;
        case VcoD: mt = static_cast<uint8_t>(core::map::module::vco); break;
        case CsoA: mt = static_cast<uint8_t>(core::map::module::cso); break;
        case CsoB: mt = static_cast<uint8_t>(core::map::module::cso); break;
    }
}

void Display::moduleMenu(core::Spiro* o, const core::map::module::type& mt, const int mp)
{
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    auto module = o->rack.at(mt, mp);
    auto sector = o->grid->getSector(mt, mp);
    auto description =  std::string(sector->options->description) + " " + std::string(1, 'A' + mp);

    core::draw_text_label(layer.get(), gtFont, description.c_str(), 30, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
    
    for(int i = 0; i < sector->options->parameters; ++i)
    {
        core::draw_text_label(layer.get(), gtFont, sector->options->parameterId[i].data(), 30, 30 + 20 * i, contrast);
        core::draw_text_label(layer.get(), gtFont, sector->options->choice[0][0].data(), 30, 50 + 20 * i, contrast);

        // auto value = core::wforms_chaotic[0];
        // core::draw_text_label(layer.get(), gtFont, value, 70, 30, contrast);
    }
    vSoft(JumpUp, StepUp, StepDown, JumpDown);
    hSoft(JumpLeft, StepLeft, StepRight, JumpRight);

    layerOn = true;
    repaint();
}

void Display::switchPage(const Page& p)
{
    // switch(p)
    // {
    //     case CroA: CROMenu(); break;
    //     case CsoA: CSOMenu(core::Module *, int); break;
    //     case A: LFOMenu(core::Module *, int); break;
    //
    // }
}

void Display::paint(juce::Graphics& g)
{
    if(page == CroA) CROMenu();

    for(int y = 0; y < area.h; y++)
    {
        for(int x = 0; x < area.w; x++)
        {
            float c = canvas.get()->get(x, y);
            float l = layer.get()->get(x, y);
            image->setPixelAt(x, y, juce::Colour::fromFloatRGBA(2.5f * (c + l) , 0.71f * (c + l) * 2, 0.20f, c+l));
            canvas.get()->set(x, y, c * 0.6f);
        }
    }
    g.drawImageAt(*image, 0, 0, false);
}

void Display::CROMenu()
{
    if(auto data = _data.lock())
    {
        int precision = 4;
        layer.get()->clr(0.0f);
        float center_y = area.h /2;
        float center_x = area.w /2;
        auto gain = 60.0f;

        // if(scope_type->load() < 0.5f)
        // {
        //     // auto raw = data->get();
        //     // prior.x = raw.x * gain + center_x;
        //     // prior.y = raw.y * gain + center_y;
        //     float x = 0, y = 0;
        //
        //     for(unsigned i = 1; i < data->segments/2; i++)
        //     {
        //         auto raw = data->get();
        //         x += raw.x;
        //         y += raw.y;
        //         if(i % precision == 0 || i == data->segments)
        //         {
        //             x /= (float)precision;
        //             y /= (float)precision;
        //             x = x * gain + center_x;
        //             y = y * gain + center_y;
        //             lineSDFAABB(canvas.get(), prior.x, prior.y, x, y, 0.8f / (i + 1), 0.01f / (i + 1));
        //             // drawLine(canvas.get(), prior.x, prior.y, x, y,  0.5f);
        //
        //             // canvas.get()->set(x, y, 1.0f);
        //             prior.x = x;
        //             prior.y = y;
        //             x = 0.0f;
        //             y = 0.0f;
        //
        //         }
        //     }
        // }
        if(scope_type->load() < 0.5f)
        {
            auto raw = data->get();
            prior.x = raw.x * gain + center_x;
            prior.y = raw.y * gain + center_y;

            for(unsigned i = 0; i < data->segments - 1; i++)
            {
                auto raw = data->get();
                float x = raw.x * gain + center_x;
                float y = raw.y * gain + center_y;
                lineSDFAABB(canvas.get(), prior.x, prior.y, x, y, 0.8f / (i + 1), 0.01f / (i + 1)) ;
                prior.x = x;
                prior.y = y;
            }
        }
        else if(scope_type->load() > 0.5f)
        {
            int queueSize = newlyPopped.size();
            for(int i = 0; i < notInterpolatedData.size(); ++i)
            {
                auto f = data->get();
                notInterpolatedData.at(i) = f.x + f.y;
            }
            interpolator.process(ratio, notInterpolatedData.data(), newlyPopped.data(), queueSize);             // resample data
            std::copy(sampleData.data() + queueSize, sampleData.data() + sampleData.size(), newData.begin());   // shift & add new data
            int n = sampleData.size() - queueSize;
            for(int i = 0; i < queueSize; ++i)
            {
                if(n >= newData.size()) n = 0;
                newData.at(n) = newlyPopped.at(i);
                ++n;
            }
            sampleData = newData;                                                                               // set data to plot equal to shifted data

            auto data_ = sampleData.data();
            auto numSamples = sampleData.size();

            for(size_t i = 1; i < numSamples; ++i) // for each point map and draw line
            {
                lineSDFAABB
                (
                    canvas.get(), 
                    juce::jmap(float(i - 1), float(0), float(numSamples - 1), 0.0f, float(area.w)),
                    center_y - gain * data_[i - 1],
                    juce::jmap(float(i), float(0), float(numSamples - 1), 0.0f, float(area.w)),
                    center_y - gain * data_[i],
                    0.8f,
                    0.01f
                );
            }
        }
        core::boxBlur(canvas.get(), 1);
        layerOn = true;
        hSoft(StepLeft, StepRight, MI, PL);
    }
    else listeners.call([this](Listener &l) { l.bufferDisconnected(); });
}


void Display::SaveMenu()
{
    page = Page::Save;

    inputBox.setVisible(true);
    layerOn = true;
    repaint();
}

void Display::LoadMenu(std::vector<std::pair<juce::String, const juce::File>>* list)
{
    files = list->size();
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);

    last_page = files / rows_max;

    int last_row = files % rows_max - 1;
    if     (load_page > last_page) load_page = 0;
    else if(load_page < 0) load_page = last_page;
    if(load_page == last_page)
    {
        if(row > last_row) row = 0;
        else if(row < 0) row = last_row;
    }
    else
    {
        if(row >= rows_max) row = 0;
        else if(row < 0) row = rows_max - 1;
    }

    juce::String lp ("LOAD PAGE: "); lp += load_page;
    core::draw_text_label(layer.get(), gtFont, lp.toRawUTF8(), 30, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
    core::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);

    for(int i = 0; i < rows_max; ++i)
    {
        int pos = i + rows_max * load_page;
        if(pos >= files) break;
        core::draw_text_label(layer.get(), gtFont, list->at(pos).first.toRawUTF8(), 46, 30 + 10 * i, contrast);
    }

    vSoft(JumpUp, StepUp, StepDown, JumpDown);
    hSoft(CX, OK, StepLeft, StepRight);
    layerOn = true;
    repaint();
}

void Display::MainMenu()
{
    page = Page::Menu;

    if     (row > 2) row = 2;
    else if(row < 0) row = 0;
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_text_label(layer.get(), gtFont, "PRESET:", 30, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
    core::draw_text_label(layer.get(), gtFont, "SAVE", 46, 30, contrast);
    core::draw_text_label(layer.get(), gtFont, "LOAD", 46, 40, contrast);
    core::draw_text_label(layer.get(), gtFont, "INIT", 46, 50, contrast);

    core::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);

    vSoft(JumpUp, StepUp, StepDown, JumpDown);
    hSoft(CX, OK, EM, EM);
    layerOn = true;
    repaint();
}

void Display::vSoft(const int a, const int b, const int c, const int d)
{
    auto step = area.h / 30;
    auto offset = area.w / 30;
    core::draw_glyph(layer.get(), gtFont, a, offset, step *  8, contrast);
    core::draw_glyph(layer.get(), gtFont, b, offset, step * 14, contrast);
    core::draw_glyph(layer.get(), gtFont, c, offset, step * 20, contrast);
    core::draw_glyph(layer.get(), gtFont, d, offset, step * 26, contrast);
}

void Display::hSoft(const int a, const int b, const int c, const int d)
{
    auto step = area.w / 30;
    auto offset = area.h - area.h / 15;
    core::draw_glyph(layer.get(), gtFont, a, step *  8, offset, contrast);
    core::draw_glyph(layer.get(), gtFont, b, step * 13, offset, contrast);
    core::draw_glyph(layer.get(), gtFont, c, step * 18, offset, contrast);
    core::draw_glyph(layer.get(), gtFont, d, step * 23, offset, contrast);
}

void Display::CSOMenu(core::Module* chs, int id)
{
    page = id == 0 ? Page::CsoA : Page::CsoB;
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_text_label(layer.get(), gtFont, (id == 0 ? "DYNAMIC SYSTEM A:" : "DYNAMIC SYSTEM B:"), 30, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);

    core::draw_text_label(layer.get(), gtFont, "TYPE: ", 30, 30, contrast);
    // core::draw_text_label(layer.get(), gtFont, core::wforms_chaotic[(int)chs->ctrl[static_cast<int>(interface::map::ctrl::form)]->load()], 70, 30, contrast);

    vSoft(JumpUp, StepUp, StepDown, JumpDown);
    hSoft(JumpLeft, StepLeft, StepRight, JumpRight);

    layerOn = true;
    repaint();
}

void Display::LFOMenu(core::Module* lfo, int id)
{
    page = id == 0 ? Page::LfoA : Page::LfoB;
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_text_label(layer.get(), gtFont, (id == 0 ? "LFO A:" : "LFO B:"), 30, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);

    core::draw_text_label(layer.get(), gtFont, "FORM: ", 30, 30, contrast);
    // core::draw_text_label(layer.get(), gtFont, core::wforms_lfo[(int)(lfo->ctrl[static_cast<int>(core::interface::lfo::ctrl::form)]->load())], 70, 30, contrast);

    vSoft(JumpUp, StepUp, StepDown, JumpDown);
    hSoft(JumpLeft, StepLeft, StepRight, JumpRight);

    layerOn = true;
    repaint();
}

void Display::OFFMenu()
{
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_text_label(layer.get(), gtFont, "SPIRO V.0.4.0-ALPHA", 0, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "", 0, 20, contrast);
    core::draw_text_label(layer.get(), gtFont, "COPYRIGHT (C) 2022-2023", 0, 30, contrast);
    core::draw_text_label(layer.get(), gtFont, "POLE ", 0, 40, contrast);
    core::draw_text_label(layer.get(), gtFont, "MIT License ", 0, 50, contrast);

    vSoft(JumpUp, StepUp, StepDown, JumpDown);
    hSoft(JumpLeft, StepLeft, StepRight, JumpRight);

    layerOn = true;
    repaint();
}



void Display::VCOMenu(core::Module* o, int id)
{
    if     (row > 2) row = 2;
    else if(row < 0) row = 0;
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);
    switch(id)
    {
        case 0:
            core::draw_text_label(layer.get(), gtFont, "OSCILLATOR A:", 30, 10, contrast);
            page = VcoA;
            break;
        case 1:
            core::draw_text_label(layer.get(), gtFont, "OSCILLATOR B:", 30, 10, contrast);
            page = VcoB;
            break;
        case 2:
            core::draw_text_label(layer.get(), gtFont, "OSCILLATOR C:", 30, 10, contrast);
            page = VcoC;
            break;
        case 3:
            core::draw_text_label(layer.get(), gtFont, "OSCILLATOR D:", 30, 10, contrast);
            page = VcoD;
            break;
        default: 
            break;
    }
    core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);

    core::draw_text_label(layer.get(), gtFont, "FORM  : ", 50, 30, contrast);
    // core::draw_text_label(layer.get(), gtFont, core::wforms_vco[(int)(o->ccv[static_cast<int>(core::vco::ctl::form)]->load())], 90, 30, contrast);

    core::draw_text_label(layer.get(), gtFont, "MODE  : ", 50, 40, contrast);

    // if(o->ccv[static_cast<int>(core::vco::ctl::freerun)]->load() > 0.5f) core::draw_text_label(layer.get(), gtFont, "JumpRightEERUN", 90, 40, contrast);
    // else core::draw_text_label(layer.get(), gtFont, "TRIGGERED", 90, 40, contrast);

    core::draw_text_label(layer.get(), gtFont, "OCTAVE: ", 50, 50, contrast);
    // juce::String oct ( *o->ccv[static_cast<int>(core::vco::ctl::octave)] * 10.0f);
    // core::draw_text_label(layer.get(), gtFont, oct.toRawUTF8(), 110, 50, contrast);

    vSoft(JumpUp, StepUp, StepDown, JumpDown);
    hSoft(JumpLeft, StepLeft, StepRight, JumpRight);
    layerOn = true;
    repaint();
}

// void Display::EnvelopeMenu(core::envelope* env, int id)
// {
//  inputBox.setVisible(false);
//  layer.get()->clr(0.0f);
//
//  switch(id)
//  {
//      case 0: core::draw_text_label(layer.get(), gtFont, "ENVELOPE A:", 30, 10, contrast); break;
//      case 1: core::draw_text_label(layer.get(), gtFont, "ENVELOPE B:", 30, 10, contrast); break;
//      case 2: core::draw_text_label(layer.get(), gtFont, "ENVELOPE C:", 30, 10, contrast); break;
//      case 3: core::draw_text_label(layer.get(), gtFont, "ENVELOPE D:", 30, 10, contrast); break;
//         default: break;
//  }
//  core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
//
//  core::draw_text_label(layer.get(), gtFont, "SCALE: ", 30, 30, contrast);
//
//  float percent = env->time_scale->load();
//  unsigned sc  = roundf(percent * 100.0f);
//  juce::String scale = juce::String::formatted("%d%%", sc);
//  core::draw_text_label(layer.get(), gtFont, scale.toRawUTF8(), 96, 30, contrast);
//
//  vSoft(JumpUp, StepUp, StepDown, JumpDown);
//  hSoft(JumpLeft, StepLeft, StepRight, JumpRight);
//
//  layerOn = true;
//  repaint();
// }


void Display::resized()
{
    inputBox.setBounds(0, 0, 1, 1);
    reset();
}

Display::Display(std::shared_ptr<core::wavering<core::Point2D<float>>> buf, const core::Rectangle<int>& area): _data(buf), area(area)
{
    image = std::make_unique<juce::Image>(juce::Image::PixelFormat::ARGB, area.w, area.h, true);
    canvas = std::make_unique<core::Canvas<float>>(area.w, area.h);
    canvas.get()->clr(0.0f);
    layer = std::make_unique<core::Canvas<float>>(area.w, area.h);
    layer.get()->clr(0.0f);
    inputBox.canvas = layer.get();
    addAndMakeVisible(inputBox);
    reset();
}

Display::~Display()
{
}

OledLabel::OledLabel(const float* c): contrast(c) 
{
    setMultiLine(false, false);
    setTabKeyUsedAsCharacter(false);
    setBorder(juce::BorderSize(0));

    setCaretVisible(false);
    setInputRestrictions(16, "");

    setColour(juce::TextEditor::ColourIds::backgroundColourId,        juce::Colour::fromRGBA(0,0,0,0));
    setColour(juce::TextEditor::ColourIds::highlightColourId,         juce::Colour::fromRGBA(0,0,0,0));
    setColour(juce::TextEditor::ColourIds::highlightedTextColourId,   juce::Colour::fromRGBA(0,0,0,0));
    setColour(juce::TextEditor::ColourIds::outlineColourId,           juce::Colour::fromRGBA(0,0,0,0));
    setColour(juce::TextEditor::ColourIds::focusedOutlineColourId,    juce::Colour::fromRGBA(0,0,0,0));
    setColour(juce::TextEditor::ColourIds::shadowColourId,            juce::Colour::fromRGBA(0,0,0,0));
}


void OledLabel::paint(juce::Graphics& g)
{
    auto area = getLocalBounds();
    canvas->clr(0.0f);
    core::draw_text_label(canvas, gtFont, "SAVE PRESET:", 30, 10, *contrast);
    core::draw_text_label(canvas, gtFont, "-------------------", 30, 20, *contrast);
    core::draw_text_label(canvas, gtFont, ">", 30, 40, *contrast);
    core::draw_text_label(canvas, gtFont, getText().toRawUTF8(), 46, 40, *contrast);
    int cp = getCaretPosition();
    core::draw_text_label(canvas, gtFont, "_", 46 + 8 * cp, 42, *contrast);

    core::draw_glyph(canvas, gtFont, CX,  49, 155, *contrast);
    core::draw_glyph(canvas, gtFont, OK,  79, 155, *contrast);
}


void Display::reset()
{
    // set attributes
    int displayLength = (int)(time_scale->load() * core::settings::sample_rate);
    ratio = (double)displayLength / (double)area.w;
    displayLength /= ratio;
    // resize buffers
    double dataLength = core::settings::sample_rate/core::settings::scope_fps / ratio;
    sampleData.resize(area.w);
    newData.resize(sampleData.size());
    newlyPopped.resize(dataLength);
    notInterpolatedData.resize(core::settings::sample_rate/core::settings::scope_fps);
    // fill all buffers with 0
    std::fill(sampleData.begin(), sampleData.end(), 0);
    std::fill(newlyPopped.begin(), newlyPopped.end(), 0);
    std::fill(notInterpolatedData.begin(), notInterpolatedData.end(), 0);
    std::fill(newData.begin(), newData.end(), 0);
    // reset interpolator
    interpolator.reset();
}
