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
#include "Display.h"
#include "Colours.hpp"
#include "blur.hpp"
#include "fonts.h"
#include "juce_graphics/juce_graphics.h"
#include <cstddef>
#include <string_view>

using namespace juce::gl;


namespace shader
{
    namespace vertex 
    {
        constexpr std::string_view passthrough(R"(
            attribute vec4 position;
            attribute vec2 texCoord;
            varying vec2 vTexCoord;
            void main() {
                vTexCoord = texCoord;
                gl_Position = position;
            })"
        );
    }

    namespace fragment 
    {
        constexpr std::string_view brt_extract(R"(
            uniform sampler2D texture;
            uniform float threshold;
            varying vec2 vTexCoord;
            void main() {
                vec4 color = texture2D(texture, vTexCoord);
                float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
                if(brightness > threshold)
                    gl_FragColor = color;
                else
                    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            })"
        );  

        constexpr std::string_view blur_gauss(R"(
            uniform sampler2D texture;
            uniform vec2 direction;
            uniform vec2 resolution;
            varying vec2 vTexCoord;
            void main() {
                vec2 off = direction / resolution;
                vec4 sum = vec4(0.0);
                sum += texture2D(texture, vTexCoord - off * 4.0) * 0.0162162162;
                sum += texture2D(texture, vTexCoord - off * 3.0) * 0.0540540541;
                sum += texture2D(texture, vTexCoord - off * 2.0) * 0.1216216216;
                sum += texture2D(texture, vTexCoord - off * 1.0) * 0.1945945946;
                sum += texture2D(texture, vTexCoord) * 0.2270270270;
                sum += texture2D(texture, vTexCoord + off * 1.0) * 0.1945945946;
                sum += texture2D(texture, vTexCoord + off * 2.0) * 0.1216216216;
                sum += texture2D(texture, vTexCoord + off * 3.0) * 0.0540540541;
                sum += texture2D(texture, vTexCoord + off * 4.0) * 0.0162162162;
                gl_FragColor = sum;
            })"
        );

        constexpr std::string_view combined(R"(
            uniform sampler2D originalTexture;
            uniform sampler2D bloomTexture;
            uniform float intensity;
            varying vec2 vTexCoord;
            void main() {
                vec4 original = texture2D(originalTexture, vTexCoord);
                vec4 bloom = texture2D(bloomTexture, vTexCoord);
                gl_FragColor = original + bloom * intensity;
            })"
        );
    }
   
}








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

void Display::openGLContextClosing()
{
    // Clean up when context is closing
    brightnessShader.reset();
    blurShader.reset();
    combineShader.reset();
    brightnessFBO.release();
    sceneFBO.release();
    bloomFBO[0].release();
    bloomFBO[1].release();
}

void Display::createShaders()
{
    brightnessShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    brightnessShader->addVertexShader(std::string(shader::vertex::passthrough));
    brightnessShader->addFragmentShader(std::string(shader::fragment::brt_extract));
    brightnessShader->link();
    brightnessShader->use();
    
    // Gaussian blur shader
    blurShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    blurShader->addVertexShader(std::string(shader::vertex::passthrough));
    blurShader->addFragmentShader(std::string(shader::fragment::blur_gauss));
    blurShader->link();
    blurShader->use();
    
    // Gaussian blur shader
    combineShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    combineShader->addVertexShader(std::string(shader::vertex::passthrough));
    combineShader->addFragmentShader(std::string(shader::fragment::combined));
    combineShader->link();
    combineShader->use();
    
}

void Display::renderQuad()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
    glEnd();
}

void Display::renderOpenGL()
{
    // sceneFBO.makeCurrentRenderingTarget();
    juce::OpenGLHelpers::clear(palette::bg_dimmed);
    renderScope3();
    
    // sceneFBO.releaseAsRenderingTarget();
    
    /*
    brightnessFBO.makeCurrentRenderingTarget();
    juce::OpenGLHelpers::clear(juce::Colours::black);


    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sceneFBO.getTextureID());
    renderQuad();
    glDisable(GL_TEXTURE_2D);
    brightnessFBO.releaseAsRenderingTarget();
    
    GLuint currentTexture = sceneFBO.getTextureID();
    */
    /* 
    for(int i = 0; i < blurPasses; ++i)
    {
        bloomFBO[0].makeCurrentRenderingTarget();
        glUseProgram(0);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, currentTexture);
        renderQuad();
        glDisable(GL_TEXTURE_2D);
        bloomFBO[0].releaseAsRenderingTarget();
        
        bloomFBO[1].makeCurrentRenderingTarget();
        glUseProgram(0);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bloomFBO[0].getTextureID());
        renderQuad();
        glDisable(GL_TEXTURE_2D);
        bloomFBO[1].releaseAsRenderingTarget();
        
        currentTexture = bloomFBO[1].getTextureID();
    }
    */
   
    /*
    juce::OpenGLHelpers::clear(palette::bg_dimmed);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    renderQuad();
    glDisable(GL_TEXTURE_2D);
    */
}

void Display::renderScope3() noexcept
{
    if(auto data = _data.lock())
    {
        float ndc_w = 2.0f / area.w;
        float ndc_h = 2.0f / area.h;
        float center_x = -1.0f + (area.w * 0.5f * ndc_w);
        float center_y = -1.0f + (area.h * 0.5f * ndc_h);
        float gain = (*scope_scale + 1.0f) * 10.0f * std::max(ndc_w, ndc_h);

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1.0f);
        glColor4f(0.65f, 0.60f, 0.30f, 0.8f);

        glBegin(GL_LINE_STRIP);

            auto raw = data->get();

            for(int i = 0; i < windowSamplesCRO3; ++i)
            {
                auto raw = data->get();
                float x = raw.x * gain + center_x;
                float y = raw.y * gain + center_y;
                glVertex2f(x, y);
            }

        glEnd();
        glDisable(GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
    
    }
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

            for(int i = 0; i < data->segments - 1; i++)
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
            for(size_t i = 0; i < notInterpolatedData.size(); ++i)
            {
                auto f = data->get();
                notInterpolatedData.at(i) = f.x + f.y;
            }

            interpolator.process(ratio, notInterpolatedData.data(), newlyPopped.data(), queueSize);             // resample data
            std::copy(sampleData.data() + queueSize, sampleData.data() + sampleData.size(), newData.begin());   // shift & add new data
            size_t n = sampleData.size() - queueSize;

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
    core::draw_text_label(layer.get(), gtFont, lp.toRawUTF8(),          grid(3, X), grid(1, Y)              , contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------",   grid(3, X), grid(2, Y)              , contrast);
    core::draw_glyph(layer.get(), gtFont, 113,                          grid(3, X), grid(3 + row[page], Y)  , contrast);

    for(int i = 0; i < rows_max; ++i)
    {
        int pos = i + rows_max * load_page;
        if(pos >= files) break;
        core::draw_text_label(layer.get(), gtFont, list->at(pos).first.toRawUTF8(), grid(4, X), grid(3 + i, Y), contrast);
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
    core::draw_text_label(layer.get(), gtFont, "PRESET:",               grid(3, X), grid(1, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "-------------------",   grid(3, X), grid(2, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "SAVE",                  grid(4, X), grid(3, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "LOAD",                  grid(4, X), grid(4, Y), contrast);
    core::draw_text_label(layer.get(), gtFont, "INIT",                  grid(4, X), grid(5, Y), contrast);

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

void Display::newOpenGLContextCreated()
{
    sceneFBO.initialise(openGLContext, area.w, area.h);
    brightnessFBO.initialise(openGLContext, area.w, area.h);

    bloomFBO[0].initialise(openGLContext, area.w, area.h);
    bloomFBO[1].initialise(openGLContext, area.w, area.h);
    
    createShaders();
};

void Display::resized()
{
    inputBox.setBounds(0, 0, 1, 1);
    reset();
    
    const auto bounds = getLocalBounds().toFloat();
    if (openGLContext.isActive()) 
    {
        framebufferTexture.loadImage (*framebuffer);  // Loads Image to GL texture [web:24]
        glFramebuffer.makeCurrentRenderingTarget();   // Binds FBO for offscreen render 
    }

    if (openGLContext.isAttached())
    {
        brightnessFBO.release();
        bloomFBO[0].release();
        bloomFBO[1].release();
        sceneFBO.release();
        
        brightnessFBO.initialise(openGLContext, getWidth(), getHeight());
        bloomFBO[0].initialise(openGLContext, getWidth(), getHeight());
        bloomFBO[1].initialise(openGLContext, getWidth(), getHeight());
        sceneFBO.initialise(openGLContext, getWidth(), getHeight());
    }
}

Display::Display(Processor* p, std::shared_ptr<core::wavering<core::Point2D<float>>> buf, const core::Rectangle<int>& area): processor(p), _data(buf), area(area)
{
    framebuffer = std::make_unique<juce::Image>(juce::Image::PixelFormat::ARGB, area.w, area.h, true);
    canvas = std::make_unique<core::Canvas<float>>(area.w, area.h);
    canvas.get()->clr(0.0f);
    layer = std::make_unique<core::Canvas<float>>(area.w, area.h);
    layer.get()->clr(0.0f);
    inputBox.canvas = layer.get();

    windowSamplesCRO3 = int(core::settings::sample_rate * (windowMsCRO3 * 0.001f));
  
    juce::OpenGLPixelFormat pixelFormat;
    pixelFormat.multisamplingLevel = 8;
    openGLContext.setPixelFormat(pixelFormat);

    setOpaque(true);
    openGLContext.setRenderer(this); 
    openGLContext.attachTo(*this);
    openGLContext.setContinuousRepainting(true);

    addAndMakeVisible(inputBox);
    reset();
}

Display::~Display()
{
    openGLContext.setContinuousRepainting(false);
    openGLContext.detach();
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
    if(g.isClipEmpty()) return; // Prevent warning

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
