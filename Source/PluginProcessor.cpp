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
    sockets = std::make_unique<Sockets>(924, 196, cell::settings::ports_in, cell::settings::ports_out );
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

    for(int i = 0; i < cell::settings::pot_n; ++i)
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

    for(int i = 0; i < cell::settings::prm_n; ++i)
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

    for(int i = 0; i < cell::settings::ports_in*cell::settings::ports_out; ++i)
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
    for(int i = 0; i < cell::settings::osc_n; ++i)
    {
        feed.renderer.vco[i].reset();
    }
    for(int i = 0; i < cell::settings::env_n; ++i)
    {
        feed.renderer.env[i].reset();
    }
    for(int i = 0; i < cell::settings::vcf_n; ++i)
    {
        feed.renderer.vcf[i].reset();
    }
    for(int i = 0; i < cell::settings::vca_n; ++i)
    {
        feed.renderer.vca[i].fuse();
    }
}

void SpiroSynth::reloadParameters()
{
    suspendProcessing(true);
    for(int i = 0; i < cell::settings::pot_n; ++i)
    {
        interface::potentiometer_list p = static_cast<interface::potentiometer_list>(i);
        feed.renderer.bus.pot[p] = tree.getRawParameterValue (slider_list.at(p).id);
    }

    for(int i = 0; i < cell::settings::prm_n; ++i)
    {
        interface::parameter_list p = static_cast<interface::parameter_list>(i);
        parameter[i] = tree.getParameter (parameter_list.at(p).id); 
        feed.renderer.bus.prm[p] = tree.getRawParameterValue (parameter_list.at(p).id);
    }

    for(int i = 0; i < cell::settings::ports_in*cell::settings::ports_out; ++i)
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
    cell::settings::buffer_size = samplesPerBlock;
    cell::settings::sample_rate = sampleRate;

    cell::settings::reset_time_multiplier();

    sockets->setBounds(juce::Rectangle<int> { 35, 238, 926, 198 });
    c_buffer = std::make_unique<cell::wavering<cell::l_r<float>>>( sampleRate/cell::settings::scope_fps);
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
        c_buffer.get()->set(cell::l_r<float> { feed.renderer.out[0], feed.renderer.out[1] });
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
