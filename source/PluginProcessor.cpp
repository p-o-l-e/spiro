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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "core/grid.hpp"
#include "core/uid.hpp"
#include "iospecs.hpp"
#include "utility.hpp"
#include "wavering.hpp"
#include <cstdint>


Processor::Processor(): AudioProcessor
                        (
                            BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                                             .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                        ),
                        spiro(new core::Grid(core::settings::create_sector_map())),
                        tree(*this, nullptr, juce::Identifier ("default"), createParameterLayout())
{
    buffer = std::make_shared<core::wavering<core::Point2D<float>>>(14);
    parameters = new juce::RangedAudioParameter*[spiro.grid->count(core::Control::parameter)];    
    matrix = new juce::AudioProcessorParameter*[spiro.grid->count(core::Control::input) * spiro.grid->count(core::Control::output)];
    sockets = std::make_unique<Sockets>(core::constraints::pbay, *spiro.grid);

    sockets->bay->on_connect = [this](uint32_t out)
    {
        auto uid = core::decode_uid(out);
        auto idx = this->spiro.rack.index(uid.mt, uid.mp);
        this->spiro.addConnection(idx);
    };

    sockets->bay->on_disconnect = [this](uint32_t out)
    {
        auto uid = core::decode_uid(out);
        auto idx = this->spiro.rack.index(uid.mt, uid.mp);
        this->spiro.removeConnection(idx);
    };

    spiro.bay = sockets->bay;

    scanPresetDir();
}

Processor::~Processor()
{
    delete[] matrix;
    delete[] parameters;
}

/******************************************************************************************************************************
* 
*  Value tree setter
* 
******************************************************************************************************************************/
juce::AudioProcessorValueTreeState::ParameterLayout Processor::createParameterLayout()
{

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    for(int i = 0; i < spiro.grid->count(core::Control::slider); ++i)
    {
        auto uid = spiro.grid->getUID(i, core::Control::slider);
        const core::Control* c = spiro.grid->control(uid);
        
        layout.add(std::make_unique<juce::AudioParameterFloat> 
        (
            spiro.grid->name(uid, true), 
            spiro.grid->name(uid, false), 
            juce::NormalisableRange<float>
            (
                c->min,
                c->max,
                c->step,
                c->skew,
                c->symmetric
            ),
            c->def
        ));
    }

    for(int i = 0; i < spiro.grid->count(core::Control::parameter); ++i)
    {
        auto uid = spiro.grid->getUID(i, core::Control::parameter);
        const core::Control* c = spiro.grid->control(uid);
        
        layout.add(std::make_unique<juce::AudioParameterFloat> 
        (
            spiro.grid->name(uid, true), 
            spiro.grid->name(uid, false), 
            c->min,
            c->max,
            c->def
        ));
    }

    for(int i = 0; i < spiro.grid->count(core::Control::button); ++i)
    {
        auto uid = spiro.grid->getUID(i, core::Control::button);
        layout.add(std::make_unique<juce::AudioParameterBool>
        (
            spiro.grid->name(uid, true), 
            spiro.grid->name(uid, false),
            false
        ));
    }

    for(int i = 0; i < spiro.grid->count(core::Control::input) * spiro.grid->count(core::Control::output); ++i)
    {
        // auto uid = spiro.grid->getUID(i, core::Control::input);
        layout.add(std::make_unique<juce::AudioParameterBool>
        (
            "mm" + juce::String(i),
            "MM" + juce::String(i),
            false

        ));
    }

    // suspendProcessing(false);
    return layout;
}

/***************************************************************************************************************************
* 
*  Preset routines
* 
**************************************************************************************************************************/
int Processor::getNumPrograms()
{
    if(presets.empty()) scanPresetDir();
    return (int)presets.size();
}

int Processor::getCurrentProgram()
{
    return currentPresetPosition;
}

void Processor::setCurrentProgram(int index)
{
    if(presets.empty()) scanPresetDir();
    if(index < 0 || index >= (int)presets.size()) return;
    currentPresetPosition = index;
    loadPreset(presets[index].first);
}

const juce::String Processor::getProgramName(int index)
{
    if(presets.empty()) scanPresetDir();
    if(index >= 0 && index < (int)presets.size())
    {
        return presets[index].first;
    }
    return juce::String {};
}

void Processor::changeProgramName(int index, const juce::String& newName)
{
    if(presets.empty()) scanPresetDir();
    if(index < 0 || index >= (int)presets.size() || newName.isEmpty()) return;

    juce::File oldFile = presets[index].second;
    if(oldFile.existsAsFile())
        oldFile.moveFileTo(oldFile.getParentDirectory().getChildFile(newName));

    presets[index].first = newName;
    currentPresetPosition = index;
    currentPresetName = newName;
    scanPresetDir();
}

juce::Result Processor::getPresetsFolder()
{
    preset_directory = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory);
    preset_directory = preset_directory.getChildFile("Spiro").getChildFile("Presets");
    juce::Result res = preset_directory.createDirectory();
    return res;
}

/***************************************************************************************************************************
* 
*  Save preset
* 
**************************************************************************************************************************/
bool Processor::savePreset(juce::String presetName, bool skipIfPresetWithThisNameExists)
{
    getPresetsFolder();

    currentPresetName = presetName;
    juce::MemoryBlock newPresetFileContent;
    getStateInformation(newPresetFileContent);

    auto presetFile = preset_directory.getChildFile(presetName);
    if(skipIfPresetWithThisNameExists && presetFile.existsAsFile())
    {
        return false;
    }


    auto tempFile = preset_directory.getChildFile("temp");

    auto file = presetFile.create();

    jassert(file.wasOk());
    if(file.failed())
    {
        return false;
    }
    file = tempFile.create();

    jassert(file.wasOk());
    if(file.failed())
    {
        return false;
    }

    if(auto stream = std::unique_ptr<juce::FileOutputStream>(tempFile.createOutputStream()))
    {
        stream->setPosition(0);
        stream->truncate();
        stream->write(newPresetFileContent.getData(), newPresetFileContent.getSize());
    }
    else
    {
        return false;
    }
    tempFile.moveFileTo(presetFile);
    tempFile.deleteFile();

    scanPresetDir();
    return true;
}


/***************************************************************************************************************************
* 
*  Load preset
* 
**************************************************************************************************************************/
bool Processor::loadPreset(juce::String presetName)
{
    scanPresetDir();
    auto presetFile = findPresetFile(presetName);
    if (presetFile.getFullPathName().isNotEmpty() && presetFile.existsAsFile())
    {
        currentPresetName = presetName;
        juce::MemoryMappedFile file(presetFile, juce::MemoryMappedFile::readOnly);
        setStateInformation(file.getData(), int(file.getSize()));
        return true;
    }
    return false;
}

void Processor::presetFilesAvailableChanged()
{
    presets.clear();
    for(auto& presetFile: presets_available)
    {
        presets.emplace_back(std::make_pair(presetFile.getFileNameWithoutExtension(), presetFile));
    }
}

const juce::File Processor::findPresetFile(const juce::String& name)
{
    auto presetFile = std::find_if 
    (
        presets.begin(), 
        presets.end(),
        [&name] (const std::pair<juce::String, const juce::File>& p)
        {
            return p.first == name;
        }
    );

    if(presetFile != presets.end()) return presetFile->second;
    return {};
}

juce::StringArray Processor::getPresetList()
{
    juce::StringArray presetList;
    presetList.ensureStorageAllocated(int(presets.size()));
    for(auto& preset: presets) presetList.add(preset.first);
    return presetList;
}

void Processor::scanPresetDir()
{
    getPresetsFolder();
    if(!preset_directory.exists()) preset_directory.createDirectory();
    const auto wildcard = juce::String("*");
    auto found = preset_directory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, wildcard);

    if(found != presets_available)
    {
        presets_available = found;
        presetFilesAvailableChanged();
    }
}


/***************************************************************************************************************************
* 
*  On save
* 
**************************************************************************************************************************/
void Processor::getStateInformation(juce::MemoryBlock& destData)
{
    saveMatrix();
    listeners.call([this](Listener &L) { L.saveCall(); });
    auto state = tree.copyState();
    state.setProperty(presetNameID, currentPresetName, nullptr);
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}


/***************************************************************************************************************************
* 
*  On load
* 
**************************************************************************************************************************/
void Processor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName(tree.state.getType()))
        {
            tree.replaceState(juce::ValueTree::fromXml(*xmlState));
            juce::String presetNameLoaded = tree.state.getProperty (presetNameID, "");
            if(presetNameLoaded.isNotEmpty())
            {
                currentPresetName = presetNameLoaded;
                for(size_t i = 0; i < presets.size(); ++i)
                {
                    if(presets[i].first == presetNameLoaded)
                    {
                        currentPresetPosition = (int)i;
                        break;
                    }
                }
            }
            // Apply the restored state to the DSP core (works headless, without an editor)
            reloadParameters();
            loadMatrix();
        }
    }
    listeners.call([this](Listener &L) { L.loadCall(); });
}

void Processor::reset()
{
    const juce::ScopedLock sl(getCallbackLock());
    spiro.reset();
    reloadParameters();
    applyMatrix();
}

void Processor::saveMatrix()
{
    if(spiro.bay == nullptr) return;
    int i = 0;
    for(int x = 0; x < spiro.grid->count(core::Control::input); ++x)
    {
        for(int y = 0; y < spiro.grid->count(core::Control::output); ++y)
        {
            matrix[i] = tree.getParameter("mm" + juce::String(i));
            if(matrix[i] != nullptr)
            {
                const float v = spiro.bay->matrix.get(x, y) ? 1.0f : 0.0f;
                if(!juce::approximatelyEqual(matrix[i]->getValue(), v))
                    matrix[i]->setValueNotifyingHost(v);
            }
            ++i;
        }
    }
}

void Processor::applyMatrix()
{
    // Re-wire the patchbay from its current matrix (live cords), without
    // re-reading the mm parameters. Used when the host re-prepares us
    // (prepareToPlay/reset), so live connections survive.
    const juce::ScopedLock sl(getCallbackLock());
    if(spiro.bay == nullptr || sockets == nullptr) return;
    sockets->load();
}

void Processor::loadMatrix()
{
    // mm parameters -> patchbay matrix -> wire. Used on state/preset recall.
    const juce::ScopedLock sl(getCallbackLock());
    if(spiro.bay == nullptr) return;
    int i = 0;
    for(int x = 0; x < spiro.grid->count(core::Control::input); ++x)
    {
        for(int y = 0; y < spiro.grid->count(core::Control::output); ++y)
        {
            matrix[i] = tree.getParameter("mm" + juce::String(i));
            const bool on = matrix[i] != nullptr && core::bool_from_range(matrix[i]->getValue());
            spiro.bay->matrix.set(x, y, on);
            ++i;
        }
    }
    applyMatrix();
}

void Processor::reloadParameters()
{
    const juce::ScopedLock sl(getCallbackLock());
    for(size_t i = 0; i < spiro.grid->count(core::Control::slider); ++i)
    {
        auto uid = spiro.grid->getUID(i, core::Control::slider);
        auto raw = tree.getRawParameterValue(spiro.grid->name(uid, true));
        spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->ccv[uid.pp] = raw;
    }
    
    for(size_t i = 0; i < spiro.grid->count(core::Control::parameter); ++i)
    {
        auto uid = spiro.grid->getUID(i, core::Control::parameter);
        auto name = spiro.grid->name(uid, true);
        auto raw = tree.getRawParameterValue(name);
        spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->ccv[uid.pp] = raw;
        parameters[i] = tree.getParameter(name);
    }

    for(size_t i = 0; i < spiro.grid->count(core::Control::input); ++i)
    {
        auto uid = spiro.grid->getUID(i, core::Control::input);
        auto idx = spiro.bay->get_index(core::encode_uid(uid));
        spiro.bay->io[idx].com = &spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->icv[uid.pp];
    }

    for(size_t i = 0; i < spiro.grid->count(core::Control::output); ++i)
    {
        auto uid = spiro.grid->getUID(i, core::Control::output);
        auto idx = spiro.bay->get_index(core::encode_uid(uid));
        spiro.bay->io[idx].data = &spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->ocv[uid.pp];
    }

    for(int i = 0; i < spiro.grid->count(core::Control::input) * spiro.grid->count(core::Control::output); ++i)
    {
        matrix[i] = tree.getParameter("mm" + juce::String(i));
    }
}

/***************************************************************************************************************************
* 
*  Prepare to play
* 
**************************************************************************************************************************/
void Processor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

    core::settings::buffer_size = samplesPerBlock;
    core::settings::sample_rate = (unsigned)sampleRate;
    if(!buffer) buffer = std::make_shared<core::wavering<core::Point2D<float>>>(14);
    {
        const juce::ScopedLock sl(getCallbackLock());
        spiro.reset();          // Rebuild sample-rate dependent module state (delay lines, smoothing, etc.)
        reloadParameters();
        applyMatrix();          // Re-wire live patchbay cords (mm params may be stale mid-session)
    }
    if(getActiveEditor())
    {
        listeners.call([this](Listener &L) { L.resetCall(); });
    }
    suspendProcessing(false);
}

void Processor::releaseResources()
{
    // Do not suspend here: hosts may keep calling processBlock after releaseResources,
    // and the wrapper outputs silence while the processor is suspended.
}

/******************************************************************************************************************************
* 
*  Processing
* 
******************************************************************************************************************************/
void Processor::processBlock(juce::AudioBuffer<float>& data, juce::MidiBuffer& midiMessages)
{
    const juce::ScopedNoDenormals noDenormals;

    data.clear();

    if(!isSuspended())
    {
        const int samples = data.getNumSamples();
        float* DataL = data.getWritePointer(0);
        float* DataR = data.getNumChannels() > 1 ? data.getWritePointer(1) : DataL;

        auto render = [&](int from, int to)
        {
            for(int i = from; i < to; ++i)
            {
                spiro.process();
                const float L = spiro.out[core::Spiro::stereo::l].load() * 0.2f;
                const float R = spiro.out[core::Spiro::stereo::r].load() * 0.2f;
                DataL[i] = L;
                DataR[i] = R;
                if(buffer) buffer->set(core::Point2D<float>{ L, R });
            }
        };

        // Apply MIDI events at their sample positions, not at the start of the block
        int pos = 0;
        for(const auto& metadata : midiMessages)
        {
            const int eventPos = juce::jlimit(0, samples, metadata.samplePosition);
            render(pos, eventPos);
            pos = eventPos;
            const uint8_t status = metadata.data[0];
            const uint8_t msb    = metadata.numBytes >= 2 ? metadata.data[1] : 0;
            const uint8_t lsb    = metadata.numBytes == 3 ? metadata.data[2] : 0;
            spiro.midiMessage(status, msb, lsb);
        }
        render(pos, samples);

        if(buffer) buffer->sync((unsigned long)samples, core::Mode::W);
    }

    midiMessages.clear();
    triggerAsyncUpdate();
}


juce::AudioProcessorEditor* Processor::createEditor()
{

    return new Editor(*this, tree);
}

void Processor::handleAsyncUpdate()
{
    Editor* editor = dynamic_cast<Editor*>(getActiveEditor());
    if(editor) editor->display->asyncUpdate();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Processor();
}

