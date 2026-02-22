#pragma once
#include "juce_opengl/juce_opengl.h"
#include "core/graphics/shader_descriptor.hpp"

class Shader
{
    private:
        const juce::OpenGLContext* context;
        const core::shader::Descriptor* vertex;
        const core::shader::Descriptor* fragment;

        juce::OpenGLShaderProgram* shaderProgram = nullptr;


        size_t nvA = 0;
        size_t nfA = 0;
        size_t nvU = 0;
        size_t nfU = 0;

    public:
        juce::OpenGLShaderProgram::Attribute** vA = nullptr;
        juce::OpenGLShaderProgram::Attribute** fA = nullptr;
        juce::OpenGLShaderProgram::Uniform**   vU = nullptr;
        juce::OpenGLShaderProgram::Uniform**   fU = nullptr;

        void use() const { shaderProgram->use(); }

        Shader(const juce::OpenGLContext*, const core::shader::Descriptor*, const core::shader::Descriptor*);
        ~Shader();
};


