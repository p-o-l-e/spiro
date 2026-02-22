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
#include "canvas.hpp"
#include "constants.hpp"
#include "fonts.h"
#include "iospecs.hpp"
#include "juce_graphics/juce_graphics.h"
#include "shader_descriptor.hpp"
#include "shapes.hpp"
#include <cstddef>
#include <cstdint>
#include <string_view>

using namespace juce::gl;

static constexpr void drawGraticule(core::Canvas<uint8_t>* canvas, uint8_t intensity = 0x5F, unsigned vdiv = 10, unsigned hdiv = 8, unsigned minGap = 5) noexcept;
static constexpr void drawBorder(core::Canvas<uint8_t>* canvas) noexcept;

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
    // hSoft(glyph::JumpLeft, glyph::StepLeft, glyph::StepRight, glyph::JumpRight, &layer.get());

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
 }

void Display::createShaders()
{
    using namespace core;

    brightnessShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    brightnessShader->addVertexShader(std::string(shader::vertex::passthrough));
    brightnessShader->addFragmentShader(std::string(shader::fragment::red_gate));
    brightnessShader->link();

    attrPos = std::make_unique<juce::OpenGLShaderProgram::Attribute>(*brightnessShader, "position");
    attrTex = std::make_unique<juce::OpenGLShaderProgram::Attribute>(*brightnessShader, "texCoord");
    uniTex  = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*brightnessShader, "tex");

    uniFgColor = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*brightnessShader, "fgColor");
    uniBgColor = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*brightnessShader, "bgColor");

    shader[ShaderType::Solid] = std::make_unique<Shader>
    (
        &openGLContext,
        &core::shader::postprocess::descriptor,
        &core::shader::solid::descriptor
    );
    


    thresholdShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    thresholdShader->addVertexShader(std::string(shader::vertex::ndc_to_uv));
    thresholdShader->addFragmentShader(std::string(shader::fragment::threshold));
    thresholdShader->link();
    
    uThresholdTex = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*thresholdShader, "tex");
    uThreshold = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*thresholdShader, "threshold");

    bloomCompositeShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    bloomCompositeShader->addVertexShader(std::string(shader::vertex::ndc_to_uv));
    bloomCompositeShader->addFragmentShader(std::string(shader::fragment::combine_add));
    bloomCompositeShader->link();

    uBaseTex  = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*bloomCompositeShader, "uTexA");
    uBloomTex = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*bloomCompositeShader, "uTexB");
   
    
    postPosThreshold = std::make_unique<juce::OpenGLShaderProgram::Attribute>(*thresholdShader, "position");
    postPosComposite = std::make_unique<juce::OpenGLShaderProgram::Attribute>(*bloomCompositeShader, "position");

    // --- HBLUR ---
    hblurShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    hblurShader->addVertexShader(std::string(shader::vertex::ndc_to_uv));
    hblurShader->addFragmentShader(std::string(shader::fragment::hblur));
    hblurShader->link();

    uHBlurTex = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*hblurShader, "tex");
    uHBlurTexelSize = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*hblurShader, "texelSize");
    postPosHBlur = std::make_unique<juce::OpenGLShaderProgram::Attribute>(*hblurShader, "position");

    // --- VBLUR ---
    vblurShader.reset(new juce::OpenGLShaderProgram(openGLContext));
    vblurShader->addVertexShader(std::string(shader::vertex::ndc_to_uv));
    vblurShader->addFragmentShader(std::string(shader::fragment::vblur));
    vblurShader->link();

    uVBlurTex = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*vblurShader, "tex");
    uVBlurTexelSize = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*vblurShader, "texelSize");
    postPosVBlur = std::make_unique<juce::OpenGLShaderProgram::Attribute>(*vblurShader, "position");


    // Afterglow
    afterglowFadeShader = std::make_unique<juce::OpenGLShaderProgram>(openGLContext);
    afterglowFadeShader->addVertexShader(std::string(shader::vertex::ndc_to_uv)); 
    afterglowFadeShader->addFragmentShader(std::string(shader::fragment::afterglow)); 
    afterglowFadeShader->link();

    uAfterglowFadeTex = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*afterglowFadeShader, "tex"); 
    uAfterglowDecay = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*afterglowFadeShader, "decay");

    afterglowAccumulateShader = std::make_unique<juce::OpenGLShaderProgram>(openGLContext);
    afterglowAccumulateShader->addVertexShader(std::string(shader::vertex::ndc_to_uv)); 
    afterglowAccumulateShader->addFragmentShader(std::string(shader::fragment::combine_add)); 
    afterglowAccumulateShader->link(); 

    uAfterglowPrevTex = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*afterglowAccumulateShader, "uTexA"); 
    uAfterglowNewTex = std::make_unique<juce::OpenGLShaderProgram::Uniform>(*afterglowAccumulateShader, "uTexB");

    createQuadBuffers();
}

void Display::createQuadBuffers()
{
    if (quadVBO == 0)
    {
        static const GLfloat verts[] =
        {
            -1.0f, -1.0f,
             1.0f, -1.0f,
            -1.0f,  1.0f,
             1.0f,  1.0f
        };

        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    }

    if (quadTBO == 0)
    {
        static const GLfloat tex[] =
        {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f
        };

        glGenBuffers(1, &quadTBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadTBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &scopeVBO);

    const float fsQuadVerts[] = {
        -1.f, -1.f,
         1.f, -1.f,
        -1.f,  1.f,
         1.f,  1.f
    };

    glGenBuffers(1, &fsQuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, fsQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fsQuadVerts), fsQuadVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Display::createBloomFBOs()
{
    const int MSAA = 8;

    glGenFramebuffers(1, &scopeFBO_MSAA);
    glBindFramebuffer(GL_FRAMEBUFFER, scopeFBO_MSAA);

    glGenRenderbuffers(1, &scopeColorBuffer_MSAA);
    glBindRenderbuffer(GL_RENDERBUFFER, scopeColorBuffer_MSAA);

    glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_RGBA8, area.w, area.h);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, scopeColorBuffer_MSAA);

    auto makeFBO = [&](GLuint& fbo, GLuint& tex)
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, area.w, area.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    };

    makeFBO(scopeFBO, scopeTex);
    makeFBO(blurFBO_H, blurTex_H);
    makeFBO(blurFBO_V, blurTex_V);

    makeFBO(afterglowFBO, afterglowTex); 
    makeFBO(afterglowTempFBO, afterglowTempTex);

    blurTexelSizeX = 1.0f / float(area.w);
    blurTexelSizeY = 1.0f / float(area.h);

}

void Display::bakeTexture(core::Canvas<uint8_t>* canvas)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_LUMINANCE,
        (GLsizei)canvas->width,
        (GLsizei)canvas->height,
        0,
        GL_LUMINANCE,
        GL_UNSIGNED_BYTE,
        canvas->raw()
    );
}

void Display::bakeTextures()
{
    core::Canvas<uint8_t> canvas(area.w, area.h);
    canvas.clr(0);
    hSoft(glyph::StepLeft, glyph::StepRight, glyph::Minus, glyph::Plus, &canvas);
    drawGraticule(&canvas);
    //drawBorder(&canvas);
    croTexture.bind();
    bakeTexture(&canvas);

}

void Display::renderBloom() noexcept
{
    glBindFramebuffer(GL_FRAMEBUFFER, afterglowTempFBO);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    afterglowFadeShader->use();
    uAfterglowFadeTex->set(0);
    uAfterglowDecay->set(0.75f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, afterglowTex);

    renderFullscreenQuad(*postPosThreshold);
   
    glBindFramebuffer(GL_FRAMEBUFFER, afterglowFBO);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    afterglowAccumulateShader->use();
    uAfterglowPrevTex->set(0);
    uAfterglowNewTex->set(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, afterglowTempTex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, scopeTex);

    renderFullscreenQuad(*postPosThreshold);

    // Threshold: afterglowTex → blurFBO_H
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBO_H);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    thresholdShader->use();
    uThresholdTex->set(0);
    uThreshold->set(0.3f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, afterglowTex);

    renderFullscreenQuad(*postPosThreshold);

    // Horizontal blur: blurTex_H ← threshold result
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBO_V);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    hblurShader->use();
    uHBlurTex->set(0);
    uHBlurTexelSize->set(1.0f / float(area.w));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blurTex_H);

    renderFullscreenQuad(*postPosHBlur);

    // Vertical blur: blurTex_V ← blurTex_H
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBO_H);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    vblurShader->use();
    uVBlurTex->set(0);
    uVBlurTexelSize->set(1.0f / float(area.h));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blurTex_V);

    renderFullscreenQuad(*postPosVBlur);

    // Composite bloom over main framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, area.w, area.h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bloomCompositeShader->use();
    uBloomTex->set(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blurTex_H);

    renderFullscreenQuad(*postPosComposite);

}

void Display::renderQuad()
{
    if (attrPos->attributeID < 0 || attrTex->attributeID < 0) return;

    brightnessShader->use();

    glActiveTexture(GL_TEXTURE0);
    croTexture.bind();
    uniTex->set(0);

    // position buffer
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glEnableVertexAttribArray(attrPos->attributeID);
    glVertexAttribPointer(attrPos->attributeID, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // texcoord buffer
    glBindBuffer(GL_ARRAY_BUFFER, quadTBO);
    glEnableVertexAttribArray(attrTex->attributeID);
    glVertexAttribPointer(attrTex->attributeID, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(attrPos->attributeID);
    glDisableVertexAttribArray(attrTex->attributeID);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uniFgColor->set(palette::cro::fg, 4);
    uniBgColor->set(palette::cro::bg, 4);
}

void Display::renderFullscreenQuad(juce::OpenGLShaderProgram::Attribute& posAttr)
{
    glBindBuffer(GL_ARRAY_BUFFER, fsQuadVBO);
    glEnableVertexAttribArray(posAttr.attributeID);
    glVertexAttribPointer(posAttr.attributeID, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(posAttr.attributeID);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Display::renderOpenGL()
{
    croMenu();
}

void Display::renderScope3(bool Skip) noexcept
{
    using namespace core::shader;
    // 1) Draw background/UI to main framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, area.w, area.h);
    renderQuad();
    // 2) Draw scope into full-res scopeFBO
    glBindFramebuffer(GL_FRAMEBUFFER, scopeFBO_MSAA);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_MULTISAMPLE);
    
    if(auto data = _data.lock())
    {
        unsigned long current = data->written();
        unsigned long samplesToRead = current - samplesLastProduced;
        samplesLastProduced = current;

        if(Skip)
        {
            data->advance(samplesToRead);
            return;
        }

        if(samplesToRead < 0x40) return;

        float gain = (*scope_scale + 1.0f) * scopeScaleMultiplier;
        float halfWidth = 0.007f;

        shader[ShaderType::Solid]->use();
        shader[ShaderType::Solid]->fU[solid::uniform::color]->set(palette::cro::fg, 4);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const unsigned long quadCount = samplesToRead - 1;
        const unsigned long vertCount = quadCount * 4;
        const GLsizeiptr byteSize = vertCount * 2 * sizeof(float);

        glBindBuffer(GL_ARRAY_BUFFER, scopeVBO);
        glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_STREAM_DRAW);

        float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if(!ptr) return;

        float prevX, prevY;
        {
            auto raw = data->get();
            prevX = raw.x * gain + ndcCenterX;
            prevY = raw.y * gain + ndcCenterY;
        }

        for(unsigned long i = 1; i < samplesToRead; ++i)
        {
            auto raw = data->get();
            float x = raw.x * gain + ndcCenterX;
            float y = raw.y * gain + ndcCenterY;

            float dx = x - prevX;
            float dy = y - prevY;

            float len = std::sqrt(dx*dx + dy*dy);
            if (len < 1e-9f) len = 1.0f;

            dx /= len;
            dy /= len;

            float nx = -dy * halfWidth;
            float ny =  dx * halfWidth;

            *ptr++ = prevX + nx; *ptr++ = prevY + ny;
            *ptr++ = prevX - nx; *ptr++ = prevY - ny;
            *ptr++ = x     + nx; *ptr++ = y     + ny;
            *ptr++ = x     - nx; *ptr++ = y     - ny;

            prevX = x;
            prevY = y;
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);

        auto attributeID = shader[ShaderType::Solid]->vA[postprocess::attribute::position]->attributeID;

        glEnableVertexAttribArray(attributeID);
        glVertexAttribPointer(attributeID, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertCount);

        glDisableVertexAttribArray(attributeID);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_BLEND);
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, scopeFBO_MSAA); 
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scopeFBO); 
    glBlitFramebuffer( 0, 0, area.w, area.h, 0, 0, area.w, area.h, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    renderBloom();

    glDisable(GL_BLEND);
}

void Display::renderScope2(bool Skip) noexcept
{
    using namespace core::shader;
    // 1) Draw background/UI to main framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, area.w, area.h);
    renderQuad();
    // 2) Draw scope into full-res scopeFBO
    glBindFramebuffer(GL_FRAMEBUFFER, scopeFBO_MSAA);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_MULTISAMPLE);    

    if(auto data = _data.lock())
    {
        unsigned long current = data->written();
        unsigned long samplesToRead = current - samplesLastProduced;
        samplesLastProduced = current;

        if(Skip)
        {
            data->advance(samplesToRead);
            return;
        }

        if(samplesToRead < 0x40) return;

        float gain = (*scope_scale + 1.0f) * scopeScaleMultiplier;
        float halfWidth = 1.0f/ (float)area.w;

        shader[ShaderType::Solid]->use();
        shader[ShaderType::Solid]->fU[solid::uniform::color]->set(palette::cro::fg, 4);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const unsigned long quadCount = samplesToRead - 1;
        const unsigned long vertCount = quadCount * 4;
        const GLsizeiptr byteSize = vertCount * 2 * sizeof(float);

        glBindBuffer(GL_ARRAY_BUFFER, scopeVBO);
        glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_STREAM_DRAW);

        float* ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if(!ptr) return;

        float prevX, prevY;
        {
            auto raw = data->get();
            prevX = -1.0f;
            prevY = raw.y * gain + ndcCenterY;
        }

        for(unsigned long i = 1; i < samplesToRead; ++i)
        {
            auto raw = data->get();
            float x = core::remap(float(i), float(0), float(samplesToRead - 1), -1.0f, 1.0f);
            float y = raw.y * gain + ndcCenterY;

            float dx = x - prevX;
            float dy = y - prevY;

            float len = std::sqrt(dx*dx + dy*dy);
            if (len < 1e-9f) len = 1.0f;

            dx /= len;
            dy /= len;

            float nx = -dy * halfWidth;
            float ny =  dx * halfWidth;

            *ptr++ = prevX + nx; *ptr++ = prevY + ny;
            *ptr++ = prevX - nx; *ptr++ = prevY - ny;
            *ptr++ = x     + nx; *ptr++ = y     + ny;
            *ptr++ = x     - nx; *ptr++ = y     - ny;

            prevX = x;
            prevY = y;
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);

        auto attributeID = shader[ShaderType::Solid]->vA[postprocess::attribute::position]->attributeID;
        glEnableVertexAttribArray(attributeID);
        glVertexAttribPointer(attributeID, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertCount);

        glDisableVertexAttribArray(attributeID);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_BLEND);
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, scopeFBO_MSAA); 
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scopeFBO); 
    glBlitFramebuffer( 0, 0, area.w, area.h, 0, 0, area.w, area.h, GL_COLOR_BUFFER_BIT, GL_LINEAR );

    renderBloom();

    glDisable(GL_BLEND);
}

void Display::croMenu()
{    
    juce::OpenGLHelpers::clear(palette::bg_dimmed);

    if(scope_type->load() < 0.5f)
    {
        renderScope3(skipScopeRender);
    }
    else if(scope_type->load() >= 0.5f)
    {
        renderScope2(skipScopeRender);
    }
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
    // hSoft(glyph::Cancel, glyph::Ok,     glyph::StepLeft, glyph::StepRight, &layer.get());
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
    // hSoft(glyph::Cancel, glyph::Ok, glyph::Empty, glyph::Empty, &layer.get());
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

void Display::hSoft(int a, int b, int c, int d, core::Canvas<uint8_t>* canvas)
{
    auto step = canvas->width / 30;
    auto offset = canvas->height - canvas->height / 17;
    core::drawGlyph(canvas, gtFont, a, step *  8 + 4, offset, opacity);
    core::drawGlyph(canvas, gtFont, b, step * 13 + 4, offset, opacity);
    core::drawGlyph(canvas, gtFont, c, step * 18 + 4, offset, opacity);
    core::drawGlyph(canvas, gtFont, d, step * 23 + 4, offset, opacity);
}

void Display::offMenu()
{
    inputBox.setVisible(false);
    layer.get()->clr(0.0f);
    core::draw_text_label(layer.get(), gtFont, "SPIRO    V.0.5.1-ALPHA", 10, 10, contrast);
    core::draw_text_label(layer.get(), gtFont, "                      ", 10, 20, contrast);
    core::draw_text_label(layer.get(), gtFont, "COPYRIGHT(C) 2022-2025", 10, 30, contrast);
    core::draw_text_label(layer.get(), gtFont, "MIT LICENSE   [ POLE ]", 10, 40, contrast);

    // hSoft(glyph::JumpLeft, glyph::StepLeft, glyph::StepRight, glyph::JumpRight, &layer.get());

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

    createShaders();
    createBloomFBOs();
    bakeTextures();


    glBindFramebuffer(GL_FRAMEBUFFER, afterglowFBO);
    glViewport(0, 0, area.w, area.h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
};

void Display::resized()
{
    inputBox.setBounds(0, 0, 1, 1);
    reset();
    
    const auto bounds = getLocalBounds().toFloat();

    ndcW = 2.0f / area.w;
    ndcH = 2.0f / area.h;
    ndcCenterX = -1.0f + (area.w * 0.5f * ndcW);
    ndcCenterY = -1.0f + (area.h * 0.5f * ndcH);
    scopeScaleMultiplier = 10.0f * std::max(ndcW, ndcH);
}

Display::Display(Processor* p, std::shared_ptr<core::wavering<core::Point2D<float>>> buf, const core::Rectangle<int>& area): processor(p), _data(buf), area(area)
{
    framebuffer = std::make_unique<juce::Image>(juce::Image::PixelFormat::ARGB, area.w, area.h, true);
    canvas = std::make_unique<core::Canvas<float>>(area.w, area.h);
    canvas.get()->clr(0.0f);
    layer = std::make_unique<core::Canvas<float>>(area.w, area.h);
    layer.get()->clr(0.0f);
    inputBox.canvas = layer.get();

    juce::OpenGLPixelFormat pixelFormat;
    pixelFormat.redBits   = 8;
    pixelFormat.greenBits = 8;
    pixelFormat.blueBits  = 8;
    pixelFormat.alphaBits = 8;   // ⭐ critical
    pixelFormat.multisamplingLevel = 8;
    openGLContext.setPixelFormat(pixelFormat);
    openGLContext.setRenderer(this); 
    openGLContext.attachTo(*this);

    setOpaque(false);

    openGLContext.setComponentPaintingEnabled(false);
    openGLContext.setContinuousRepainting(false);
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

}

static constexpr void drawGraticule(core::Canvas<uint8_t>* canvas, uint8_t intensity, unsigned vdiv, unsigned hdiv, unsigned minGap) noexcept
{
    const float W = float(canvas->width);
    const float H = float(canvas->height);
    // usable area after enforcing minGap
    const float usableW = std::max(0.0f, W - 2.0f * float(minGap));
    const float usableH = std::max(0.0f, H - 2.0f * float(minGap));
    // enforce square divisions
    const float stepX = usableW / vdiv;
    const float stepY = usableH / hdiv;
    const float step  = std::min(stepX, stepY);
    // actual grid size
    const float gridW = step * vdiv;
    const float gridH = step * hdiv;
    // center the grid inside the minGap box
    const float hgap = (W - gridW) * 0.5f;
    const float vgap = (H - gridH) * 0.5f;
    // center coordinates
    const float cx = hgap + gridW * 0.5f;
    const float cy = vgap + gridH * 0.5f;
    // minor tick spacing
    const float sub = step / 5.0f;
    // Major vertical lines
    for (unsigned i = 0; i <= vdiv; ++i)
    {
        float x = hgap + i * step;
        int xi = int(std::round(x));
        core::drawVLine(canvas, xi, int(std::round(vgap)), int(std::round(vgap + gridH)), intensity);
    }
    // Major horizontal lines
    for (unsigned i = 0; i <= hdiv; ++i)
    {
        float y = vgap + i * step;
        int yi = int(std::round(y));
        core::drawHLine(canvas, int(std::round(hgap)), yi, int(std::round(hgap + gridW)), intensity);
    }
    // Minor vertical ticks
    const float tickH = step * 0.2f;

    for (unsigned i = 0; i < vdiv * 5; ++i)
    {
        float x = hgap + i * sub;
        int xi = int(std::round(x));

        core::drawVLine(canvas, xi, int(std::round(cy - tickH)), int(std::round(cy + tickH)), intensity);
    }
    // Minor horizontal ticks
    const float tickW = step * 0.2f;

    for (unsigned i = 0; i < hdiv * 5; ++i)
    {
        float y = vgap + i * sub;
        int yi = int(std::round(y));

        core::drawHLine(canvas, int(std::round(cx - tickW)), yi, int(std::round(cx + tickW)), intensity);
    }
    // Dotted lines at ±2.4 divisions
    const float dottedOffset = step * 2.4f;

    const int yt = int(std::round(cy - dottedOffset));
    const int yb = int(std::round(cy + dottedOffset));

    for (unsigned i = 0; i < vdiv * 5; ++i)
    {
        int x = int(std::round(hgap + i * sub));
        canvas->set(x, yt, intensity);
        canvas->set(x, yb, intensity);
    }
}

static constexpr void drawBorder(core::Canvas<uint8_t>* canvas) noexcept
{
    core::drawHLine(canvas, 0, 0, canvas->width, 0xFF);
    core::drawHLine(canvas, 0, canvas->height - 1, canvas->width, 0xFF);
    core::drawVLine(canvas, 0, 0, canvas->height, 0xFF);
    core::drawVLine(canvas, canvas->width - 1, 0, canvas->height, 0xFF);
}
