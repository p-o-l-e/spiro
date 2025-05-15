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

#pragma once
#include <JuceHeader.h>
#include "cell/containers.hpp"
#include "cell/primitives.hpp"
#include "cell/fonts.h"
#include "cell/blur.hpp"
#include "cell/chaos.hpp"
#include "cell/oscillator.hpp"
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
		float contrast = 0.6f;
		cell::frame<float>* canvas;
		cell::rectangle<int> area;
		void paint (juce::Graphics& g) override;
		OledLabel() {};
	   ~OledLabel() {};
};


class Display : public juce::ImageComponent
{
	private:
		std::unique_ptr<juce::Image> 		image;
		std::unique_ptr<cell::frame<float>> canvas;
		std::unique_ptr<cell::frame<float>> layer;
		int lx = 0, ly = 0;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Display)
	public:
		cell::wavering<cell::point2d<float>>*	data;
		OledLabel input_box;
		int load_page = 0;
		int last_page = 0;
		int rows_max = 10;
		int files = 0;


		std::atomic<float>* scope_type = &cell::zero;
		std::atomic<float>* scope_scale = &cell::zero;
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
		float contrast = 0.6f;
		cell::rectangle<int> area;
		void paint (juce::Graphics& g) override;
        void Scope();
		void ChaosMenu(cell::map_t*, int);
		void EnvelopeMenu(cell::envelope*, int);
		void VCOMenu(cell::oscillator*, int);
		void LFOMenu(cell::lfo_t*, int);
		void MainMenu();
		void SaveMenu();
        void About();
		void vWind(int, int, int, int);
		void hWind(int, int, int, int);
		void LoadMenu(std::vector<std::pair<juce::String, const juce::File>>*);
		void resized() override;
		void reset();
		Display(cell::wavering<cell::point2d<float>>*, int, int, int, int);
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






