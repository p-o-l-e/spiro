/*****************************************************************************************************************************
* Copyright (c) 2022-2026 POLE
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
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "canvas.hpp"
#include "JuceShader.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include <string_view>

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
    OledLabel();
   ~OledLabel() = default;
};

class Display: public juce::Component, private juce::OpenGLRenderer
{
    public:
		enum Page {	VcoA, VcoB,	VcoC, VcoD,	CsoA, CsoB,	LfoA, LfoB, EnvA, EnvB, EnvC, EnvD,	Save, Load,	CroA, Main, Info, COUNT };
        enum ShaderType 
        {
            Solid,
            Threshold,
            HBlur,
            VBlur,
            CombineAdd,
            Afterglow,
            AfterglowAccu,
            Size
        };

	private:
        Processor *processor;
		std::unique_ptr<juce::Image> framebuffer;
		core::Canvas<uint8_t>* textLayer;

        std::unique_ptr<Shader> shader[ShaderType::Size];

        std::unique_ptr<juce::OpenGLShaderProgram> brightnessShader;
        // Threshold
        std::unique_ptr<juce::OpenGLShaderProgram> thresholdShader;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uThresholdTex; 
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uThreshold;
        // --- Horizontal blur ---
        std::unique_ptr<juce::OpenGLShaderProgram> hblurShader; 
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uHBlurTex; 
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uHBlurTexelSize; 
        // --- Vertical blur --- 
        std::unique_ptr<juce::OpenGLShaderProgram> vblurShader; 
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uVBlurTex; 
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uVBlurTexelSize;
        // Composite pass
        std::unique_ptr<juce::OpenGLShaderProgram> bloomCompositeShader;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uBloomTex;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uBaseTex;
        // Afterglow
        std::unique_ptr<juce::OpenGLShaderProgram> afterglowFadeShader;
        std::unique_ptr<juce::OpenGLShaderProgram> afterglowAccumulateShader;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uAfterglowFadeTex;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uAfterglowDecay;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uAfterglowPrevTex;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uAfterglowNewTex;

        std::unique_ptr<juce::OpenGLShaderProgram::Attribute> attrPos; 
        std::unique_ptr<juce::OpenGLShaderProgram::Attribute> attrTex;
        std::unique_ptr<juce::OpenGLShaderProgram::Attribute> postPosHBlur; 
        std::unique_ptr<juce::OpenGLShaderProgram::Attribute> postPosVBlur;
        std::unique_ptr<juce::OpenGLShaderProgram::Attribute> postPosThreshold;
        std::unique_ptr<juce::OpenGLShaderProgram::Attribute> postPosComposite;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniTex;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniFgColor;
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniBgColor;

        juce::OpenGLTexture croTexture;
        juce::OpenGLTexture moduleTexture;

        GLuint scopeVBO;
        GLuint scopeFBO, scopeTex; 
        GLuint blurFBO_H, blurTex_H; 
        GLuint blurFBO_V, blurTex_V;
        GLuint fsQuadVBO;
        GLuint quadVBO = 0;
        GLuint quadTBO = 0;
        GLuint afterglowFBO = 0; 
        GLuint afterglowTex = 0; 
        GLuint afterglowTempFBO = 0; 
        GLuint afterglowTempTex = 0;
        GLuint moduleTextureID;

        GLuint scopeFBO_MSAA; GLuint scopeColorBuffer_MSAA;
        float blurTexelSizeX = 0.0f;   // 1.0f / area.w
        float blurTexelSizeY = 0.0f;   // 1.0f / area.h

        void renderBloom() noexcept;

        const float contrast = 0.6f;
        const uint8_t opacity = 0xAF;
        std::atomic<bool> needsUpload { true };
        bool repaintTexture = true;
		int last_page = 0;
        int stepX = 10, stepY = 10;

        float ndcW;
        float ndcH;
        float ndcCenterX;
        float ndcCenterY;
        float scopeScaleMultiplier;
        unsigned long samplesLastProduced = 0;
        bool skipScopeRender = false;

        constexpr static bool X = true, Y = false;
        constexpr int grid(const int v, const bool axis) const { return axis? v * stepX: v * stepY; }
        void bakeTexture(juce::OpenGLTexture*, core::Canvas<uint8_t>*);
        void createShaders();
        void createBloomFBOs();
        void createQuadBuffers();
        void renderQuad(juce::OpenGLTexture*);
        void renderFullscreenQuad(juce::OpenGLShaderProgram::Attribute&);
        void skipFrames();
        void renderScope2() noexcept;
        void renderScope3() noexcept;
        void renderModuleMenu() noexcept;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Display)

	public:
        std::weak_ptr<core::wavering<core::Point2D<float>>> _data;
	    OledLabel inputBox;
        juce::OpenGLContext openGLContext;
		const int rows_max = 12;
		int files = 0;

		std::atomic<float>* scope_type = &core::zero;
		std::atomic<float>* scope_scale = &core::zero;
		std::atomic<float>  ts { 0.02f };
		std::atomic<float>* time_scale = &ts;

		Page page = CroA;
        const core::uid_t getUID() const;
		int row[Page::COUNT] = {};
		int load_page = 0;
        core::uid_t uid;
		const core::Rectangle<int> area;
        void switchPage(Processor*, const Page); 
	
        void offMenu();
        void croMenu();
    	void moduleMenu(core::Spiro*, const core::map::module::type&, const int);
		void mainMenu();
		void saveMenu();
		void drawSoftGlyphsV(int, int, int, int, core::Canvas<uint8_t>*);
		void drawSoftGlyphsH(int, int, int, int, core::Canvas<uint8_t>*);
		void loadMenu(std::vector<std::pair<juce::String, const juce::File>>*);
		void resized() override;
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

        virtual void newOpenGLContextCreated() override;
        virtual void renderOpenGL() override;
        virtual void openGLContextClosing() override;

};

