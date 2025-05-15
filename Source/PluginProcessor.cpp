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

SpiroSynth::SpiroSynth():   AudioProcessor
                            (
                                BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                                                 .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)),
                                                 tree(*this, nullptr, juce::Identifier ("default"), createParameterLayout()
                            )

{
    suspendProcessing(true);
    sockets = std::make_unique<Sockets>(924, 196, core::settings::ports_in, core::settings::ports_out );
    feed.renderer.bay = sockets->bay;
}

SpiroSynth::~SpiroSynth()
{
}

/******************************************************************************************************************************
* 
* 
*  Value tree setter
* 
* 
******************************************************************************************************************************/
juce::AudioProcessorValueTreeState::ParameterLayout SpiroSynth::createParameterLayout()
{
    suspendProcessing(true);
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    for(int i = 0; i < core::settings::pot_n; ++i)
    {
        interface::potentiometer_list p = static_cast<interface::potentiometer_list>(i);
        layout.add (std::make_unique<juce::AudioParameterFloat> 
        (
            slider_list.at(p).id, 
            slider_list.at(p).name, 
            juce::NormalisableRange<float>
            (
                slider_list.at(p).min,
                slider_list.at(p).max,
                slider_list.at(p).interval,
                slider_list.at(p).skew,
                slider_list.at(p).symmetric
            ),
            slider_list.at(p).def
        ));
    }

    for(int i = 0; i < core::settings::prm_n; ++i)
    {
        interface::parameter_list p = static_cast<interface::parameter_list>(i);
        layout.add (std::make_unique<juce::AudioParameterFloat> 
        (
            parameter_list.at(p).id, 
            parameter_list.at(p).name, 
            parameter_list.at(p).min,
            parameter_list.at(p).max,
            parameter_list.at(p).def
        ));
    }

    for(int i = 0; i < core::settings::ports_in*core::settings::ports_out; ++i)
    {
        layout.add (std::make_unique<juce::AudioParameterFloat> 
        (
            "matrix_" + juce::String(i), 
            "Matrix_" + juce::String(i), 
            0.0f,
            1.0f,
            0.0f
        ));
    }

    suspendProcessing(false);
    return layout;
}

/***************************************************************************************************************************
* 
* 
*  Preset routines
* 
* 
**************************************************************************************************************************/
int SpiroSynth::getNumPrograms() { return presets.size(); }
int SpiroSynth::getCurrentProgram() { return 0; }
void SpiroSynth::setCurrentProgram (int index) {}
const juce::String SpiroSynth::getProgramName (int index) { return {}; }
void SpiroSynth::changeProgramName (int index, const juce::String& newName) {}

juce::Result SpiroSynth::getPresetsFolder()
{
    preset_directory = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory);
    preset_directory = preset_directory.getChildFile("Spiro").getChildFile("Presets");
    juce::Result res = preset_directory.createDirectory();
    return res;
}

/***************************************************************************************************************************
* 
* 
*  Save preset
* 
* 
**************************************************************************************************************************/
bool SpiroSynth::savePreset(juce::String presetName, bool skipIfPresetWithThisNameExists)
{

    suspendProcessing(true);
    getPresetsFolder();

    currentPresetName = presetName;
    juce::MemoryBlock newPresetFileContent;
    getStateInformation (newPresetFileContent);

    auto presetFile = preset_directory.getChildFile(presetName);
    auto tempFile = preset_directory.getChildFile("temp");

    auto file = presetFile.create();

    jassert (file.wasOk());
    if (file.failed())
    {
        suspendProcessing(false);
        return false;
    }
    file = tempFile.create();

    jassert (file.wasOk());
    if (file.failed()) 
    {
        suspendProcessing(false);
        return false;
    }

    if (auto stream = std::unique_ptr<juce::FileOutputStream> (tempFile.createOutputStream()))
    {
        stream->setPosition(0);
        stream->truncate();
        stream->write (newPresetFileContent.getData(), newPresetFileContent.getSize());
    }
    else 
    {
        suspendProcessing(false);
        return false;
    }
    tempFile.moveFileTo(presetFile);
    tempFile.deleteFile();

    suspendProcessing(false);
    return true;
}


/***************************************************************************************************************************
* 
* 
*  Load preset
* 
* 
**************************************************************************************************************************/
bool SpiroSynth::loadPreset(juce::String presetName)
{

    suspendProcessing(true);
    scanPresetDir();
    auto presetFile = findPresetFile (presetName);
    if (presetFile.getFullPathName().isNotEmpty() && presetFile.existsAsFile())
    {
        currentPresetName = presetName;

        juce::MemoryMappedFile file (presetFile, juce::MemoryMappedFile::readOnly);

        setStateInformation (file.getData(), int (file.getSize()));

        suspendProcessing(false);
        return true;
    }

    suspendProcessing(false);
    return false;
}

void SpiroSynth::presetFilesAvailableChanged()
{
    presets.clear();

    for (auto& presetFile : presets_available)
    {
        presets.emplace_back (std::make_pair (presetFile.getFileNameWithoutExtension(), presetFile));
    }
}

const juce::File SpiroSynth::findPresetFile (const juce::String& presetNameToLookFor)
{
    auto presetFile = std::find_if 
    (
        presets.begin(), 
        presets.end(),
        [&presetNameToLookFor] (const std::pair<juce::String, const juce::File>& p)
        {
            return p.first == presetNameToLookFor;
        }
    );

    if (presetFile != presets.end()) return presetFile->second;
    return {};
}

juce::StringArray SpiroSynth::getPresetList()
{
    juce::StringArray presetList;
    presetList.ensureStorageAllocated (int (presets.size()));
    for (auto& preset : presets) presetList.add (preset.first);
    return presetList;
}

void SpiroSynth::scanPresetDir()
{
    getPresetsFolder();
    if (!preset_directory.exists()) preset_directory.createDirectory();
    const auto fileExtensionWildcard = juce::String ("*");
    auto allPresetsFound = preset_directory.findChildFiles (juce::File::TypesOfFileToFind::findFiles, false, fileExtensionWildcard);

    if (allPresetsFound != presets_available)
    {
        presets_available = allPresetsFound;
        presetFilesAvailableChanged();
    }
}


/***************************************************************************************************************************
* 
* 
*  On save
* 
* 
**************************************************************************************************************************/
void SpiroSynth::getStateInformation (juce::MemoryBlock& destData)
{
    listeners.call([this](Listener &l) { l.saveCall(); });
    auto state = tree.copyState();
    state.setProperty (presetNameID, currentPresetName, nullptr);

    auto xml = state.createXml();
    copyXmlToBinary (*xml, destData);
}


/***************************************************************************************************************************
* 
* 
*  On load
* 
* 
**************************************************************************************************************************/
void SpiroSynth::setStateInformation (const void* data, int sizeInBytes)
{
    auto xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState.get() != nullptr) juce::ScopedLock scopedLock (parametersLock);

    if (xmlState->hasTagName (tree.state.getType()))
    {
        reset();
        tree.replaceState (juce::ValueTree::fromXml (*xmlState));
        juce::String presetNameLoaded = tree.state.getProperty (presetNameID, "");
    }
    listeners.call([this](Listener &l) { l.loadCall(); });
}

void SpiroSynth::reset()
{
    for(int i = 0; i < core::settings::osc_n; ++i)
    {
        feed.renderer.vco[i].reset();
    }
    for(int i = 0; i < core::settings::env_n; ++i)
    {
        feed.renderer.env[i].reset();
    }
    for(int i = 0; i < core::settings::vcf_n; ++i)
    {
        feed.renderer.vcf[i].reset();
    }
    for(int i = 0; i < core::settings::vca_n; ++i)
    {
        feed.renderer.vca[i].fuse();
    }
}

void SpiroSynth::reloadParameters()
{
    suspendProcessing(true);
    for(int i = 0; i < core::settings::pot_n; ++i)
    {
        interface::potentiometer_list p = static_cast<interface::potentiometer_list>(i);
        feed.renderer.bus.pot[p] = tree.getRawParameterValue (slider_list.at(p).id);
    }

    for(int i = 0; i < core::settings::prm_n; ++i)
    {
        interface::parameter_list p = static_cast<interface::parameter_list>(i);
        parameter[i] = tree.getParameter (parameter_list.at(p).id); 
        feed.renderer.bus.prm[p] = tree.getRawParameterValue (parameter_list.at(p).id);
    }

    for(int i = 0; i < core::settings::ports_in*core::settings::ports_out; ++i)
    {
        matrix[i] = tree.getParameter("matrix_" + juce::String(i)); 
        feed.renderer.bus.mtx[i] = tree.getRawParameterValue ("matrix_" + juce::String(i));
    }

    feed.renderer.connect_bus();
    suspendProcessing(false);
}



/***************************************************************************************************************************
* 
* 
*  Prepare to play 
* 
* 
**************************************************************************************************************************/
void SpiroSynth::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    core::settings::buffer_size = samplesPerBlock;
    core::settings::sample_rate = sampleRate;

    core::settings::reset_time_multiplier();

    sockets->setBounds(juce::Rectangle<int> { 35, 238, 926, 198 });
    c_buffer = std::make_unique<core::wavering<core::Point2D<float>>>( sampleRate/core::settings::scope_fps);
    reset();
    reloadParameters();
    armed = true;

    suspendProcessing(false);
}

void SpiroSynth::releaseResources()
{
    armed = false;
    suspendProcessing(true);
}

/******************************************************************************************************************************
* 
* 
*  MIDI
* 
* 
******************************************************************************************************************************/
void SpiroSynth::handleMIDI(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (const auto metadata : midiMessages) 
    {
        uint8_t status  = metadata.data[0];
        uint8_t msb = (metadata.numBytes >= 2) ? metadata.data[1] : 0;
        uint8_t lsb = (metadata.numBytes == 3) ? metadata.data[2] : 0;
        feed.midi_message(status, msb, lsb);
    }
    midiMessages.clear();
}

/******************************************************************************************************************************
* 
* 
*  Processing
* 
* 
******************************************************************************************************************************/
void SpiroSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    handleMIDI(buffer, midiMessages);
    
    buffer.clear();
    int numSamples = buffer.getNumSamples();
 
	float* DataL    = buffer.getWritePointer(0);
	float* DataR    = buffer.getWritePointer(1);

    for (int i = 0; i < numSamples; i++)
	{
        feed.renderer.process();
        c_buffer.get()->set(core::Point2D<float> { feed.renderer.out[0], feed.renderer.out[1] });
		DataL[i] = feed.renderer.out[0] * 0.2f;
		DataR[i] = feed.renderer.out[1] * 0.2f;
	}
}


juce::AudioProcessorEditor* SpiroSynth::createEditor()
{
    suspendProcessing(true);
    return new SpiroSynthEditor (*this, tree);
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpiroSynth();
}
