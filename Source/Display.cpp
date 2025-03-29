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

#include "Display.h"

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
    if(data)
    {
        layer.get()->clr(0.0f);
        float cy = area.h /2;
        float cx = area.w /2;
        auto gain = area.h * scope_scale->load();
        if(scope_type->load() < 0.5f)
        {
            auto lp = data->get();
            lx = lp.x * gain + cx;
            ly = lp.y * gain + cy;
            for(unsigned i = 0; i < data->segments; i++)
            {
                cell::point2d<float> f = data->get();
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
        cell::blur_box(canvas.get(), 1);
        layer_on = true;
        hWind(SL, SR, MI, PL);
    }
    else listeners.call([this](Listener &l) { l.bufferDisconnected(); });
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
	cell::draw_text_label(layer.get(), gtFont, lp.toRawUTF8(), 30, 10, contrast);
	cell::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
	cell::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);

	for(int i = 0; i < rows_max; ++i)
	{
		int pos = i + rows_max * load_page;
		if(pos >= files) break;
		cell::draw_text_label(layer.get(), gtFont, list->at(pos).first.toRawUTF8(), 46, 30 + 10 * i, contrast);
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
	cell::draw_text_label(layer.get(), gtFont, "PRESET:", 30, 10, contrast);
	cell::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);
	cell::draw_text_label(layer.get(), gtFont, "SAVE", 46, 30, contrast);
	cell::draw_text_label(layer.get(), gtFont, "LOAD", 46, 40, contrast);
	cell::draw_text_label(layer.get(), gtFont, "INIT", 46, 50, contrast);

	cell::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);

	vWind(FU, SU, SD, FD);
	hWind(CX, OK, EM, EM);
	layer_on = true;
	repaint();
}

void Display::vWind(int a, int b, int c, int d)
{
	cell::draw_glyph(layer.get(), gtFont, a, 6,  39, contrast);
	cell::draw_glyph(layer.get(), gtFont, b, 6,  69, contrast);
	cell::draw_glyph(layer.get(), gtFont, c, 6,  99, contrast);
	cell::draw_glyph(layer.get(), gtFont, d, 6, 129, contrast);
}

void Display::hWind(int a, int b, int c, int d)
{
	cell::draw_glyph(layer.get(), gtFont, a,  49, 155, contrast);
	cell::draw_glyph(layer.get(), gtFont, b,  79, 155, contrast);
	cell::draw_glyph(layer.get(), gtFont, c, 109, 155, contrast);
	cell::draw_glyph(layer.get(), gtFont, d, 139, 155, contrast);
}

void Display::ChaosMenu(cell::map_t* chs, int id)
{
	page = id == 0 ? page_t::chs_a : page_t::chs_b;
	input_box.setVisible(false);
	layer.get()->clr(0.0f);
	cell::draw_text_label(layer.get(), gtFont, (id == 0 ? "DYNAMIC SYSTEM A:" : "DYNAMIC SYSTEM B:"), 30, 10, contrast);
	cell::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);

	cell::draw_text_label(layer.get(), gtFont, "TYPE: ", 30, 30, contrast);
	cell::draw_text_label(layer.get(), gtFont, cell::wforms_chaotic[(int)chs->ctrl[static_cast<int>(interface::map::ctrl::form)]->load()], 70, 30, contrast);

	vWind(FU, SU, SD, FD);
	hWind(FL, SL, SR, FR);

	layer_on = true;
	repaint();
}

void Display::LFOMenu(cell::lfo_t* lfo, int id)
{
	page = id == 0 ? page_t::lfo_a : page_t::lfo_b;
	input_box.setVisible(false);
	layer.get()->clr(0.0f);
	cell::draw_text_label(layer.get(), gtFont, (id == 0 ? "LFO A:" : "LFO B:"), 30, 10, contrast);
	cell::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);

	cell::draw_text_label(layer.get(), gtFont, "FORM: ", 30, 30, contrast);
	cell::draw_text_label(layer.get(), gtFont, cell::wforms_lfo[(int)(lfo->ctrl[static_cast<int>(cell::interface::lfo::ctrl::form)]->load())], 70, 30, contrast);

	vWind(FU, SU, SD, FD);
	hWind(FL, SL, SR, FR);

	layer_on = true;
	repaint();
}

void Display::About()
{
	input_box.setVisible(false);
	layer.get()->clr(0.0f);
	cell::draw_text_label(layer.get(), gtFont, "SPIRO V.0.4.0-ALPHA", 0, 10, contrast);
	cell::draw_text_label(layer.get(), gtFont, "", 0, 20, contrast);
	cell::draw_text_label(layer.get(), gtFont, "COPYRIGHT (C) 2022-2023", 0, 30, contrast);
	cell::draw_text_label(layer.get(), gtFont, "POLE ", 0, 40, contrast);
	cell::draw_text_label(layer.get(), gtFont, "MIT License ", 0, 50, contrast);
  


	vWind(FU, SU, SD, FD);
	hWind(FL, SL, SR, FR);

	layer_on = true;
	repaint();
}



void Display::VCOMenu(cell::oscillator* o, int id)
{
	if     (row > 2) row = 2;
	else if(row < 0) row = 0;
	input_box.setVisible(false);
	layer.get()->clr(0.0f);
	cell::draw_glyph(layer.get(), gtFont, 113, 30, 30 + row * 10, contrast);
	switch(id)
	{
		case 0:
			cell::draw_text_label(layer.get(), gtFont, "OSCILLATOR A:", 30, 10, contrast);
			page = vco_a;
			break;
		case 1:
			cell::draw_text_label(layer.get(), gtFont, "OSCILLATOR B:", 30, 10, contrast);
			page = vco_b;
			break;
		case 2:
			cell::draw_text_label(layer.get(), gtFont, "OSCILLATOR C:", 30, 10, contrast);
			page = vco_c;
			break;
		case 3:
			cell::draw_text_label(layer.get(), gtFont, "OSCILLATOR D:", 30, 10, contrast);
			page = vco_d;
			break;
		default: 
			break;
	}
	cell::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);

	cell::draw_text_label(layer.get(), gtFont, "FORM: ", 50, 30, contrast);
	cell::draw_text_label(layer.get(), gtFont, cell::wforms_vco[(int)(o->ctrl[static_cast<int>(cell::interface::vco::ctrl::form)]->load())], 90, 30, contrast);

	cell::draw_text_label(layer.get(), gtFont, "MODE: ", 50, 40, contrast);

    if(o->ctrl[static_cast<int>(cell::interface::vco::ctrl::freerun)]->load() > 0.5f) cell::draw_text_label(layer.get(), gtFont, "FREERUN", 90, 40, contrast);
	else cell::draw_text_label(layer.get(), gtFont, "TRIGGERED", 90, 40, contrast);

	cell::draw_text_label(layer.get(), gtFont, "OCTAVE: ", 50, 50, contrast);
	juce::String oct ( *o->ctrl[static_cast<int>(cell::interface::vco::ctrl::octave)] * 10.0f);
	cell::draw_text_label(layer.get(), gtFont, oct.toRawUTF8(), 110, 50, contrast);

	vWind(FU, SU, SD, FD);
	hWind(FL, SL, SR, FR);
	layer_on = true;
	repaint();
}

void Display::EnvelopeMenu(cell::envelope* env, int id)
{
	input_box.setVisible(false);
	layer.get()->clr(0.0f);
	
	switch(id)
	{
		case 0: cell::draw_text_label(layer.get(), gtFont, "ENVELOPE A:", 30, 10, contrast); break;
		case 1: cell::draw_text_label(layer.get(), gtFont, "ENVELOPE B:", 30, 10, contrast); break;
		case 2:	cell::draw_text_label(layer.get(), gtFont, "ENVELOPE C:", 30, 10, contrast); break;
		case 3:	cell::draw_text_label(layer.get(), gtFont, "ENVELOPE D:", 30, 10, contrast); break;
        default: break;
	}
	cell::draw_text_label(layer.get(), gtFont, "-------------------", 30, 20, contrast);

	cell::draw_text_label(layer.get(), gtFont, "SCALE: ", 30, 30, contrast);

	float percent = env->time_scale->load();
	unsigned sc  = roundf(percent * 100.0f);
	juce::String scale = juce::String::formatted("%d%%", sc);
	cell::draw_text_label(layer.get(), gtFont, scale.toRawUTF8(), 96, 30, contrast);

	vWind(FU, SU, SD, FD);
	hWind(FL, SL, SR, FR);

	layer_on = true;
	repaint();
}


void Display::resized()
{
	input_box.setBounds(0, 0, 1, 1);
	reset();
}

Display::Display(cell::wavering<cell::point2d<float>>* buf, int x, int y, int w, int h): data(buf)
{
	area.x = x;
	area.y = y;
	area.w = w;
	area.h = h;
	image = std::make_unique<juce::Image>(juce::Image::PixelFormat::ARGB, area.w, area.h, true);
	canvas = std::make_unique<cell::frame<float>>(area.w, area.h);
	canvas.get()->clr(0.0f);
	layer = std::make_unique<cell::frame<float>>(area.w, area.h);
	layer.get()->clr(0.0f);

	input_box.canvas = layer.get();
	input_box.setMultiLine(false, false);
	input_box.setTabKeyUsedAsCharacter(false);
	input_box.setBorder(juce::BorderSize(0));

	input_box.setCaretVisible(false);
	input_box.setInputRestrictions(16, "");

	input_box.setColour(juce::TextEditor::ColourIds::backgroundColourId,        juce::Colour::fromRGBA(0,0,0,0));
	input_box.setColour(juce::TextEditor::ColourIds::highlightColourId,         juce::Colour::fromRGBA(0,0,0,0));
	input_box.setColour(juce::TextEditor::ColourIds::highlightedTextColourId,   juce::Colour::fromRGBA(0,0,0,0));
	input_box.setColour(juce::TextEditor::ColourIds::outlineColourId,           juce::Colour::fromRGBA(0,0,0,0));
	input_box.setColour(juce::TextEditor::ColourIds::focusedOutlineColourId,    juce::Colour::fromRGBA(0,0,0,0));
	input_box.setColour(juce::TextEditor::ColourIds::shadowColourId,            juce::Colour::fromRGBA(0,0,0,0));

	addAndMakeVisible(input_box);
	reset();
}

Display::~Display()
{
}


void OledLabel::paint(juce::Graphics& g)
{
	auto area = getLocalBounds();
	canvas->clr(0.0f);
	cell::draw_text_label(canvas, gtFont, "SAVE PRESET:", 30, 10, contrast);
	cell::draw_text_label(canvas, gtFont, "-------------------", 30, 20, contrast);
	cell::draw_text_label(canvas, gtFont, ">", 30, 40, contrast);
	cell::draw_text_label(canvas, gtFont, getText().toRawUTF8(), 46, 40, contrast);
	int cp = getCaretPosition();
	cell::draw_text_label(canvas, gtFont, "_", 46 + 8 * cp, 42, contrast);

	cell::draw_glyph(canvas, gtFont, CX,  49, 155, contrast);
	cell::draw_glyph(canvas, gtFont, OK,  79, 155, contrast);
}


void Display::reset()
{
    // set attributes
    displayLength = (int)(time_scale->load() * cell::settings::sample_rate);
    ratio = (double)displayLength / (double)area.w;
    displayLength /= ratio;
    // resize buffers
    double dataLength = cell::settings::sample_rate/cell::settings::scope_fps / ratio;
    sampleData.resize(area.w);
    newData.resize(sampleData.size());
    newlyPopped.resize(dataLength);
    notInterpolatedData.resize(cell::settings::sample_rate/cell::settings::scope_fps);
    // fill all buffers with 0
    std::fill(sampleData.begin(), sampleData.end(), 0);
    std::fill(newlyPopped.begin(), newlyPopped.end(), 0);
    std::fill(notInterpolatedData.begin(), notInterpolatedData.end(), 0);
    std::fill(newData.begin(), newData.end(), 0);
    // reset interpolator
    interpolator.reset();
}
