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
#include "PluginProcessor.h"

namespace glyph
{
    constexpr int JumpUp    = 117;
    constexpr int StepUp    = 119;
    constexpr int StepDown  = 120;
    constexpr int JumpDown  = 118;
    constexpr int JumpLeft  = 121;
    constexpr int StepLeft  = 123;
    constexpr int StepRight = 124;
    constexpr int JumpRight = 122;
    constexpr int Ok        = 47;
    constexpr int Cancel    = 56;
    constexpr int Plus      = 11;
    constexpr int Minus     = 13;
    constexpr int Empty     = 125;
    constexpr int Square    = 113;
}

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
		enum Page {	VcoA, VcoB,	VcoC, VcoD,	CsoA, CsoB,	LfoA, LfoB, EnvA, EnvB, EnvC, EnvD,	Save, Load,	CroA, MainMenu, About, COUNT };

	private:
        Processor *processor;
		OledLabel inputBox { &contrast };
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
        int stepX = 10, stepY = 10;
        constexpr static bool X = true, Y = false;
        constexpr int grid(const int v, const bool axis) const { return axis? v * stepX: v * stepY; }
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Display)

	public:
        std::weak_ptr<core::wavering<core::Point2D<float>>> _data;

		bool layerOn = false;
		const int rows_max = 10;
		int files = 0;

		std::atomic<float>* scope_type = &core::zero;
		std::atomic<float>* scope_scale = &core::zero;
		std::atomic<float>  ts { 0.02f };
		std::atomic<float>* time_scale = &ts;

		Page page = CroA;
        const core::uid_t getUID() const;
		int row[Page::COUNT] = {};
        core::uid_t uid;
		const core::Rectangle<int> area;
        void switchPage(Processor*, const Page); 
		void paint(juce::Graphics& g) override;
        void offMenu();
        void croMenu();
    	void moduleMenu(core::Spiro*, const core::map::module::type&, const int);
		void mainMenu();
		void saveMenu();
		void vSoft(const int, const int, const int, const int);
		void hSoft(const int, const int, const int, const int);
		void loadMenu(std::vector<std::pair<juce::String, const juce::File>>*);
		void resized() override;
		void reset();
		Display(Processor*, std::shared_ptr<core::wavering<core::Point2D<float>>>, const core::Rectangle<int>&);
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






