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
#include <memory>

void Display::paint(juce::Graphics& g)
{
    if(page == scope) Scope();

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

void Display::Scope()
{
    std::cout<<"Display::Scope()\n";
    if(auto data = _data.lock())
    {
        layer.get()->clr(0.0f);
        float cy = area.h /2;
        float cx = area.w /2;
        // auto gain = area.h * scope_scale->load();
        auto gain = 30.0f;
        if(scope_type->load() < 0.5f)
        {
            
            auto lp = data->get();
            lx = lp.x * gain + cx;
            ly = lp.y * gain + cy;
            // std::cout<<"Scope type A | x: "<<lp.x<<"\t| y: "<<lp.y<<"\n";

            for(unsigned i = 0; i < data->segments; i++)
            {
                core::Point2D<float> f = data->get();
                float x = f.x * gain * 2.0f + cx;
                float y = f.y * gain * 2.0f + cy;
                lineSDFAABB(canvas.get(), lx, ly, x, y, 0.8f/(i+1), 0.01f/(i+1)) ;
                canvas.get()->set(x, y, 1.0f);
                lx = x;
                ly = y;
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
            // resample data
            interpolator.process(ratio, notInterpolatedData.data(), newlyPopped.data(), queueSize);
            // shift & add new data
            std::copy(sampleData.data() + queueSize, sampleData.data() + sampleData.size(), newData.begin());
            int n = sampleData.size() - queueSize;
            for(int i = 0; i < queueSize; ++i)
            {
                if(n >= newData.size()) n = 0;
                newData.at(n) = newlyPopped.at(i);
                ++n;
            }
            // set data to plot equal to shifted data
            sampleData = newData;

            auto data_ = sampleData.data();
            auto numSamples = sampleData.size();

            // for each point map and draw line
            for (size_t i = 1; i < numSamples; ++i)
            {
                lineSDFAABB
                (
                    canvas.get(), 
                    juce::jmap(float(i - 1), float(0), float(numSamples - 1), 0.0f, float(area.w)),
                    cy - gain * data_[i - 1],
                    juce::jmap(float(i), float(0), float(numSamples - 1), 0.0f, float(area.w)),
                    cy - gain * data_[i],
                    0.8f,
                    0.01f
                );
            }
        }
        core::boxBlur(canvas.get(), 1);
        layer_on = true;
        hWind(SL, SR, MI, PL);
    }
    else listeners.call([this](Listener &l) { l.bufferDisconnected(); });
    std::cout<<"eof Display::Scope()\n";

}


void Display::SaveMenu()
{
	page = page_t::save;

	input_box.setVisible(true);
	layer_on = true;
	repaint();
}

void Display::LoadMenu(std::vector<std::pair<juce::String, const juce::File>>* list)
{
	files = list->size();
	input_box.setVisible(false);
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

	vWind(FU, SU, SD, FD);
	hWind(CX, OK, SL, SR);
	layer_on = true;
	repaint();
}

void Display::MainMenu()
{
	page = page_t::menu;

	if     (row > 2) row = 2;
	else if(row < 0) row = 0;
	input_box.setVisible(false);
	layer.get()->clr(0.0f);
	core::draw_text_label(layer.get(), gtFont, "PRESET:", 30, 10, contrast);
	core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
	core::draw_text_label(layer.get(), gtFont, "SAVE", 46, 30, contrast);
	core::draw_text_label(layer.get(), gtFont, "LOAD", 46, 40, contrast);
	core::draw_text_label(layer.get(), gtFont, "INIT", 46, 50, contrast);

	core::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);

	vWind(FU, SU, SD, FD);
	hWind(CX, OK, EM, EM);
	layer_on = true;
	repaint();
}

void Display::vWind(int a, int b, int c, int d)
{
	core::draw_glyph(layer.get(), gtFont, a, 6,  39, contrast);
	core::draw_glyph(layer.get(), gtFont, b, 6,  69, contrast);
	core::draw_glyph(layer.get(), gtFont, c, 6,  99, contrast);
	core::draw_glyph(layer.get(), gtFont, d, 6, 129, contrast);
}

void Display::hWind(int a, int b, int c, int d)
{
	core::draw_glyph(layer.get(), gtFont, a,  49, 155, contrast);
	core::draw_glyph(layer.get(), gtFont, b,  79, 155, contrast);
	core::draw_glyph(layer.get(), gtFont, c, 109, 155, contrast);
	core::draw_glyph(layer.get(), gtFont, d, 139, 155, contrast);
}

// void Display::ChaosMenu(core::map_t* chs, int id)
// {
// 	page = id == 0 ? page_t::chs_a : page_t::chs_b;
// 	input_box.setVisible(false);
// 	layer.get()->clr(0.0f);
// 	core::draw_text_label(layer.get(), gtFont, (id == 0 ? "DYNAMIC SYSTEM A:" : "DYNAMIC SYSTEM B:"), 30, 10, contrast);
// 	core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
//
// 	core::draw_text_label(layer.get(), gtFont, "TYPE: ", 30, 30, contrast);
// 	core::draw_text_label(layer.get(), gtFont, core::wforms_chaotic[(int)chs->ctrl[static_cast<int>(interface::map::ctrl::form)]->load()], 70, 30, contrast);
//
// 	vWind(FU, SU, SD, FD);
// 	hWind(FL, SL, SR, FR);
//
// 	layer_on = true;
// 	repaint();
// }

// void Display::LFOMenu(core::lfo_t* lfo, int id)
// {
// 	page = id == 0 ? page_t::lfo_a : page_t::lfo_b;
// 	input_box.setVisible(false);
// 	layer.get()->clr(0.0f);
// 	core::draw_text_label(layer.get(), gtFont, (id == 0 ? "LFO A:" : "LFO B:"), 30, 10, contrast);
// 	core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
//
// 	core::draw_text_label(layer.get(), gtFont, "FORM: ", 30, 30, contrast);
// 	core::draw_text_label(layer.get(), gtFont, core::wforms_lfo[(int)(lfo->ctrl[static_cast<int>(core::interface::lfo::ctrl::form)]->load())], 70, 30, contrast);
//
// 	vWind(FU, SU, SD, FD);
// 	hWind(FL, SL, SR, FR);
//
// 	layer_on = true;
// 	repaint();
// }

void Display::About()
{
	input_box.setVisible(false);
	layer.get()->clr(0.0f);
	core::draw_text_label(layer.get(), gtFont, "SPIRO V.0.4.0-ALPHA", 0, 10, contrast);
	core::draw_text_label(layer.get(), gtFont, "", 0, 20, contrast);
	core::draw_text_label(layer.get(), gtFont, "COPYRIGHT (C) 2022-2023", 0, 30, contrast);
	core::draw_text_label(layer.get(), gtFont, "POLE ", 0, 40, contrast);
	core::draw_text_label(layer.get(), gtFont, "MIT License ", 0, 50, contrast);



	vWind(FU, SU, SD, FD);
	hWind(FL, SL, SR, FR);

	layer_on = true;
	repaint();
}



// void Display::VCOMenu(core::oscillator* o, int id)
// {
// 	if     (row > 2) row = 2;
// 	else if(row < 0) row = 0;
// 	input_box.setVisible(false);
// 	layer.get()->clr(0.0f);
// 	core::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);
// 	switch(id)
// 	{
// 		case 0:
// 			core::draw_text_label(layer.get(), gtFont, "OSCILLATOR A:", 30, 10, contrast);
// 			page = vco_a;
// 			break;
// 		case 1:
// 			core::draw_text_label(layer.get(), gtFont, "OSCILLATOR B:", 30, 10, contrast);
// 			page = vco_b;
// 			break;
// 		case 2:
// 			core::draw_text_label(layer.get(), gtFont, "OSCILLATOR C:", 30, 10, contrast);
// 			page = vco_c;
// 			break;
// 		case 3:
// 			core::draw_text_label(layer.get(), gtFont, "OSCILLATOR D:", 30, 10, contrast);
// 			page = vco_d;
// 			break;
// 		default: 
// 			break;
// 	}
// 	core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
//
// 	core::draw_text_label(layer.get(), gtFont, "FORM: ", 50, 30, contrast);
// 	core::draw_text_label(layer.get(), gtFont, core::wforms_vco[(int)(o->ctrl[static_cast<int>(core::interface::vco::ctrl::form)]->load())], 90, 30, contrast);
//
// 	core::draw_text_label(layer.get(), gtFont, "MODE: ", 50, 40, contrast);
//
//     if(o->ctrl[static_cast<int>(core::interface::vco::ctrl::freerun)]->load() > 0.5f) core::draw_text_label(layer.get(), gtFont, "FREERUN", 90, 40, contrast);
// 	else core::draw_text_label(layer.get(), gtFont, "TRIGGERED", 90, 40, contrast);
//
// 	core::draw_text_label(layer.get(), gtFont, "OCTAVE: ", 50, 50, contrast);
// 	juce::String oct ( *o->ctrl[static_cast<int>(core::interface::vco::ctrl::octave)] * 10.0f);
// 	core::draw_text_label(layer.get(), gtFont, oct.toRawUTF8(), 110, 50, contrast);
//
// 	vWind(FU, SU, SD, FD);
// 	hWind(FL, SL, SR, FR);
// 	layer_on = true;
// 	repaint();
// }

// void Display::EnvelopeMenu(core::envelope* env, int id)
// {
// 	input_box.setVisible(false);
// 	layer.get()->clr(0.0f);
//
// 	switch(id)
// 	{
// 		case 0: core::draw_text_label(layer.get(), gtFont, "ENVELOPE A:", 30, 10, contrast); break;
// 		case 1: core::draw_text_label(layer.get(), gtFont, "ENVELOPE B:", 30, 10, contrast); break;
// 		case 2:	core::draw_text_label(layer.get(), gtFont, "ENVELOPE C:", 30, 10, contrast); break;
// 		case 3:	core::draw_text_label(layer.get(), gtFont, "ENVELOPE D:", 30, 10, contrast); break;
//         default: break;
// 	}
// 	core::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
//
// 	core::draw_text_label(layer.get(), gtFont, "SCALE: ", 30, 30, contrast);
//
// 	float percent = env->time_scale->load();
// 	unsigned sc  = roundf(percent * 100.0f);
// 	juce::String scale = juce::String::formatted("%d%%", sc);
// 	core::draw_text_label(layer.get(), gtFont, scale.toRawUTF8(), 96, 30, contrast);
//
// 	vWind(FU, SU, SD, FD);
// 	hWind(FL, SL, SR, FR);
//
// 	layer_on = true;
// 	repaint();
// }


void Display::resized()
{
	input_box.setBounds(0, 0, 1, 1);
	reset();
}

    Display::Display(std::shared_ptr<core::wavering<core::Point2D<float>>> buf, int x, int y, int w, int h): _data(buf)
{
	area.x = x;
	area.y = y;
	area.w = w;
	area.h = h;
	image = std::make_unique<juce::Image>(juce::Image::PixelFormat::ARGB, area.w, area.h, true);
	canvas = std::make_unique<core::Canvas<float>>(area.w, area.h);
	canvas.get()->clr(0.0f);
	layer = std::make_unique<core::Canvas<float>>(area.w, area.h);
	layer.get()->clr(0.0f);
    input_box.canvas = layer.get();
	addAndMakeVisible(input_box);
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
    displayLength = (int)(time_scale->load() * core::settings::sample_rate);
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
