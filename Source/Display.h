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
#include "descriptor.hxx"
#include "node.hpp"
#include "uid.hpp"
#include "utility.hpp"
#include "wavering.hpp"
#include "primitives.hpp"
#include "fonts.h"
#include "blur.hpp"
#include "cso.hpp"
#include "vco.hpp"
#include "PluginProcessor.h"

#define JumpUp      117
#define StepUp      119
#define StepDown    120
#define JumpDown    118

#define JumpLeft    121
#define StepLeft    123
#define StepRight   124
#define JumpRight   122

#define OK 47
#define CX 56

#define PL 11
#define MI 13

#define EM 125

struct OledLabel: public juce::TextEditor
{
    const float* contrast;
    core::Canvas<float>* canvas;
    core::Rectangle<int> area;
    void paint (juce::Graphics& g) override;
    OledLabel(const float*);
   ~OledLabel() = default;
};


class Display: public juce::ImageComponent
{
    public:
		enum Page {	VcoA, VcoB,	VcoC, VcoD,	CsoA, CsoB,	LfoA, LfoB, EnvA, EnvB, EnvC, EnvD,	Save, Load,	CroA, Menu, About };

	private:
		OledLabel input_box { &contrast };
		std::unique_ptr<juce::Image> image;
		std::unique_ptr<core::Canvas<float>> canvas;
		std::unique_ptr<core::Canvas<float>> layer;
		std::vector<float> sampleData;                  // Data currently displayed
		std::vector<float> newlyPopped;                 // Last popped array
		std::vector<float> notInterpolatedData;         // Raw new data
		std::vector<float> newData;                     // Interpolated new data
		juce::Interpolators::Linear interpolator;
        core::Point2D<int> prior {};
		double ratio = 1.0f;
        const float contrast = 0.6f;
		int load_page = 0;
		int last_page = 0;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Display)

	public:
        std::weak_ptr<core::wavering<core::Point2D<float>>> _data;

		bool layerOn = false;
		const int rows_max = 10;
		int files = 0;

		std::atomic<float>* scope_type = &core::zero;
		// std::atomic<float>* scope_scale = &core::zero;
		std::atomic<float>  ts { 0.02f };
		std::atomic<float>* time_scale = &ts;

		Page page = CroA;
        const core::uid_t getUID() const;
		int row = 0;
		const core::Rectangle<int> area;
        void switchPage(const Page&);
		void paint (juce::Graphics& g) override;
        void OFFMenu();
        void CROMenu();
		void CSOMenu(core::Module*, int);
		void VCOMenu(core::Module*, int);
		void LFOMenu(core::Module*, int);
    	void moduleMenu(const core::map::module::type&, const int);
		void MainMenu();
		void SaveMenu();
		void vSoft(const int, const int, const int, const int);
		void hSoft(const int, const int, const int, const int);
		void LoadMenu(std::vector<std::pair<juce::String, const juce::File>>*);
		void resized() override;
		void reset();
		Display(std::shared_ptr<core::wavering<core::Point2D<float>>>, const core::Rectangle<int>&);
	   ~Display();
	   	class Listener 
        {
            public:
                virtual ~Listener() = default;
                virtual void bufferDisconnected() {};
        };
        void addListener(Listener *l) { listeners.add(l); }
        void removeListener(Listener *l) { listeners.remove(l); }
        juce::ListenerList<Listener> listeners;

};






