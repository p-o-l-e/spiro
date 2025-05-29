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

#pragma once
#include <JuceHeader.h>
#include <memory>
#include "canvas.hpp"
#include "wavering.hpp"
#include "primitives.hpp"
#include "fonts.h"
#include "blur.hpp"
#include "map.hpp"
#include "vco.hpp"
#include "PluginProcessor.h"

#define FU 117
#define SU 119
#define SD 120
#define FD 118

#define FL 121
#define SL 123
#define SR 124
#define FR 122

#define OK 47
#define CX 56

#define PL 11
#define MI 13

#define EM 125

class OledLabel : public juce::TextEditor
{
	private:

	public:
		const float* contrast;
		core::Canvas<float>* canvas;
		core::Rectangle<int> area;
		void paint (juce::Graphics& g) override;
		OledLabel(const float*);
	   ~OledLabel() {};
};


class Display : public juce::ImageComponent
{
	private:
		std::unique_ptr<juce::Image> image;
		std::unique_ptr<core::Canvas<float>> canvas;
		std::unique_ptr<core::Canvas<float>> layer;
		int lx = 0, ly = 0;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Display)

	public:
        float contrast = 0.6f;
        std::weak_ptr<core::wavering<core::Point2D<float>>> _data;
		OledLabel input_box { &contrast };
		int load_page = 0;
		int last_page = 0;
		int rows_max = 10;
		int files = 0;


		std::atomic<float>* scope_type = &core::zero;
		std::atomic<float>* scope_scale = &core::zero;
		std::atomic<float>  ts { 0.02f };
		std::atomic<float>* time_scale = &ts;

		/////////////////////////////////////////////////////////////////////////////////
		std::vector<float> sampleData; /**< Data currently displayed */
		//==============================================================================
		int displayLength;
		std::vector<float> newlyPopped;         /**< Last popped array */
		std::vector<float> notInterpolatedData; /** < Raw new data*/
		std::vector<float> newData;             /** < Interpolated new data*/
		double ratio = 1.0f;
		juce::Interpolators::Linear interpolator;
  		/////////////////////////////////////////////////////////////////////////////////	
		enum page_t
		{
			vco_a,
			vco_b,
			vco_c,
			vco_d,
			chs_a,
			chs_b,
			lfo_a,
			lfo_b,
			save,
			load,
			scope,
			envs,
			menu,
            about
		};

		bool layer_on = false;
		page_t page = scope;
		int   row = 0;
		core::Rectangle<int> area;
		void paint (juce::Graphics& g) override;
        void Scope();
		// void ChaosMenu(core::map_t*, int);
		// void EnvelopeMenu(core::envelope*, int);
		// void VCOMenu(core::oscillator*, int);
		// void LFOMenu(core::lfo_t*, int);
		void MainMenu();
		void SaveMenu();
        void About();
		void vWind(int, int, int, int);
		void hWind(int, int, int, int);
		void LoadMenu(std::vector<std::pair<juce::String, const juce::File>>*);
		void resized() override;
		void reset();
		Display(std::shared_ptr<core::wavering<core::Point2D<float>>>, int, int, int, int);
	   ~Display();

	   	class Listener 
        {
            public:
                virtual ~Listener() = default;
                virtual void bufferDisconnected() {};

        };
        void addListener(Listener *l)       { listeners.add(l);     }
        void removeListener(Listener *l)    { listeners.remove(l);  }
        juce::ListenerList<Listener> listeners;

};






