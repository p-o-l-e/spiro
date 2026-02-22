#include "JuceShader.hpp"

Shader::Shader(const juce::OpenGLContext* context,
               const core::shader::Descriptor* vertex,
               const core::shader::Descriptor* fragment)
    : context(context), vertex(vertex), fragment(fragment)
{
    shaderProgram = new juce::OpenGLShaderProgram(*context);
    shaderProgram->addVertexShader(std::string(vertex->shader));
    shaderProgram->addFragmentShader(std::string(fragment->shader));
    shaderProgram->link();

    nvA = vertex->attribute.size();
    nfA = fragment->attribute.size();
    nvU = vertex->uniform.size();
    nfU = fragment->uniform.size();

    vA = nvA ? new juce::OpenGLShaderProgram::Attribute*[nvA] : nullptr;
    fA = nfA ? new juce::OpenGLShaderProgram::Attribute*[nfA] : nullptr;
    vU = nvU ? new juce::OpenGLShaderProgram::Uniform*[nvU] : nullptr;
    fU = nfU ? new juce::OpenGLShaderProgram::Uniform*[nfU] : nullptr;

    size_t i = 0;
    for (auto n : vertex->attribute)
        vA[i++] = new juce::OpenGLShaderProgram::Attribute(*shaderProgram, n.data());

    i = 0;
    for (auto n : fragment->attribute)
        fA[i++] = new juce::OpenGLShaderProgram::Attribute(*shaderProgram, n.data());

    i = 0;
    for (auto n : vertex->uniform)
        vU[i++] = new juce::OpenGLShaderProgram::Uniform(*shaderProgram, n.data());

    i = 0;
    for (auto n : fragment->uniform)
        fU[i++] = new juce::OpenGLShaderProgram::Uniform(*shaderProgram, n.data());
}

Shader::~Shader()
{
    for (size_t i = 0; i < nvA; ++i) delete vA[i];
    for (size_t i = 0; i < nfA; ++i) delete fA[i];
    for (size_t i = 0; i < nvU; ++i) delete vU[i];
    for (size_t i = 0; i < nfU; ++i) delete fU[i];

    delete[] vA;
    delete[] fA;
    delete[] vU;
    delete[] fU;

    delete shaderProgram;
}
