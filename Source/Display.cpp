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
#include "blur.hpp"
#include "fonts.h"

void Display::switchPage(Processor* o, const Page p)
{
    page = p;
    switch(page)
    {
        case VcoA: moduleMenu(&o->spiro, core::map::module::vco, 0); break;
        case VcoB: moduleMenu(&o->spiro, core::map::module::vco, 1); break;
        case VcoC: moduleMenu(&o->spiro, core::map::module::vco, 2); break;
        case VcoD: moduleMenu(&o->spiro, core::map::module::vco, 3); break;
        case CsoA: moduleMenu(&o->spiro, core::map::module::cso, 0); break;
        case CsoB: moduleMenu(&o->spiro, core::map::module::cso, 1); break;
        case LfoA: moduleMenu(&o->spiro, core::map::module::lfo, 0); break;
        case LfoB: moduleMenu(&o->spiro, core::map::module::lfo, 1); break;
        // case CroA: croMenu(); break;
        case Load: loadMenu(&o->presets); break;
        case MainMenu: mainMenu(); break;
        default: break;
    }
}

void Display::moduleMenu(core::Spiro* o, const core::map::module::type& mt, const int mp)
{
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    auto module = o->rack.at(mt, mp);
    auto sector = o->grid->getSector(mt, mp);
    if(row[page] >= sector->options->parameters) row[page] = sector->options->parameters - 1;
    else if(row[page] < 0) row[page] = 0;
    auto description = std::string(sector->options->description) + " " + std::string(1, 'A' + mp);

    core::draw_text_label(layer.get(), gtFont, description.c_str(),   grid(3, X), grid(1, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", grid(3, X), grid(2, Y), contrast);
   
    for(int i = 0, cid = 0; i < sector->options->parameters; ++i)
    {
        auto parameter = sector->options->parameterId[i];
        core::draw_text_label(layer.get(), gtFont, parameter.data(), grid(4, X), grid(3, Y) + grid(i, Y), contrast);
        int offset = grid(parameter.size(), X) + grid(3, X);
        if(sector->options->parameterType[i] == core::Options::Choice)
        {
            auto p = static_cast<int>(*module->ccv[sector->options->parameterPosition[i]]);
            parameter = sector->options->choice[cid][p];
            core::draw_text_label(layer.get(), gtFont, parameter.data(), offset, grid(3, Y) + grid(i, Y), contrast);
            ++cid;
        }
        else if(sector->options->parameterType[i] == core::Options::Integer) 
        {
            auto p = static_cast<int>(*module->ccv[sector->options->parameterPosition[i]]);
            core::draw_text_label(layer.get(), gtFont, std::to_string(p).c_str(), offset, grid(3, Y) + grid(i, Y), contrast);
        }
    }
    
    core::draw_glyph(layer.get(), gtFont, glyph::Square, grid(3, X), grid(3, Y) + grid(row[page], Y), contrast);
    vSoft(glyph::JumpUp,   glyph::StepUp,   glyph::StepDown,  glyph::JumpDown);
    hSoft(glyph::JumpLeft, glyph::StepLeft, glyph::StepRight, glyph::JumpRight);

    uid.mt = mt;
    uid.mp = mp;
    uid.pt = core::map::cv::c;
    uid.pp = sector->options->parameterPosition[row[page]];

    layerOn = true;
    repaint();
}



void Display::paint(juce::Graphics& g)
{
    if(page == CroA) [[likely]] croMenu();

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

void Display::croMenu()
{
    if(auto data = _data.lock())
    {
        layer.get()->clr(0.0f);
        float center_y = area.h / 2;
        float center_x = area.w / 2;
        auto gain = (*scope_scale + 1.0f) * 10.0f;

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
        hSoft(glyph::StepLeft, glyph::StepRight, glyph::Minus, glyph::Plus);
    }
    else listeners.call([this](Listener &l) { l.bufferDisconnected(); });
}


void Display::saveMenu()
{
    page = Page::Save;

    inputBox.setVisible(true);
    layerOn = true;
    repaint();
}

void Display::loadMenu(std::vector<std::pair<juce::String, const juce::File>>* list)
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
        if(row[page] > last_row) row[page] = 0;
        else if(row[page] < 0) row[page] = last_row;
    }
    else
    {
        if(row[page] >= rows_max) row[page] = 0;
        else if(row[page] < 0) row[page] = rows_max - 1;
    }

    juce::String lp ("LOAD PAGE: "); lp += load_page;
    core::draw_text_label(layer.get(), gtFont, lp.toRawUTF8(), 30, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
    core::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row[page] * 10, contrast);

    for(int i = 0; i < rows_max; ++i)
    {
        int pos = i + rows_max * load_page;
        if(pos >= files) break;
        core::draw_text_label(layer.get(), gtFont, list->at(pos).first.toRawUTF8(), 46, 30 + 10 * i, contrast);
    }

    vSoft(glyph::JumpUp, glyph::StepUp, glyph::StepDown, glyph::JumpDown);
    hSoft(glyph::Cancel, glyph::Ok,     glyph::StepLeft, glyph::StepRight);
    layerOn = true;
    repaint();
}

void Display::mainMenu()
{
    page = Page::MainMenu;

    if     (row[page] > 2) row[page] = 2;
    else if(row[page] < 0) row[page] = 0;
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_text_label(layer.get(), gtFont, "PRESET:", grid(3, X), grid(1, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------", grid(3, X), grid(2, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "SAVE", grid(4, X), grid(3, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "LOAD", grid(4, X), grid(4, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "INIT", grid(4, X), grid(5, Y), contrast);

    core::draw_glyph(layer.get(), gtFont, glyph::Square, grid(3, X), grid(3, Y) + grid(row[page], Y), contrast);

    vSoft(glyph::JumpUp, glyph::StepUp, glyph::StepDown, glyph::JumpDown);
    hSoft(glyph::Cancel, glyph::Ok, glyph::Empty, glyph::Empty);
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



void Display::offMenu()
{
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_text_label(layer.get(), gtFont, "SPIRO    V.0.5.1-ALPHA", 10, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "                      ", 10, 20, contrast);
    core::draw_text_label(layer.get(), gtFont, "COPYRIGHT(C) 2022-2025", 10, 30, contrast);
    core::draw_text_label(layer.get(), gtFont, "MIT LICENSE   [ POLE ]", 10, 40, contrast);

    hSoft(glyph::JumpLeft, glyph::StepLeft, glyph::StepRight, glyph::JumpRight);

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
//  vSoft(glyph::JumpUp, glyph::StepUp, glyph::StepDown, glyph::JumpDown);
//  hSoft(glyph::JumpLeft, glyph::StepLeft, glyph::StepRight, glyph::JumpRight);
//
//  layerOn = true;
//  repaint();
// }


void Display::resized()
{
    inputBox.setBounds(0, 0, 1, 1);
    reset();
}

Display::Display(Processor* p, std::shared_ptr<core::wavering<core::Point2D<float>>> buf, const core::Rectangle<int>& area): processor(p), _data(buf), area(area)
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

    core::draw_glyph(canvas, gtFont, glyph::Cancel,  49, 155, *contrast);
    core::draw_glyph(canvas, gtFont, glyph::Ok,  79, 155, *contrast);
}


void Display::reset()
{
    // set attributes
    int displayLength = (int)(time_scale->load() * core::settings::sample_rate);
    ratio = (double)displayLength / (double)area.w;
    displayLength /= ratio;
    // resize buffers
    double dataLength = area.w;//core::settings::sample_rate/core::settings::scope_fps / ratio;
    sampleData.resize(dataLength);
    newData.resize(dataLength);
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
