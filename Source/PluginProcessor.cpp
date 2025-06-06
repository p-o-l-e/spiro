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


Processor::Processor(): AudioProcessor
                        (
                            BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                                             .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)),
                                              tree(*this, nullptr, juce::Identifier ("default"), createParameterLayout()
                        ),
                        spiro(&core::grid)

{
    suspendProcessing(true);
    std::cout<<"Processor::Processor()\n"; 
    parameters = new juce::RangedAudioParameter*[core::grid.count(core::Control::parameter)];    
    matrix = new juce::AudioProcessorParameter*[core::grid.count(core::Control::input) * core::grid.count(core::Control::output)];
    sockets = std::make_unique<Sockets>(core::constraints::pbay, core::grid);
    spiro.bay = sockets->bay;
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
    std::cout<<"-- Processor: createParameterLayout()\n";

    suspendProcessing(true);
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    for(int i = 0; i < core::grid.count(core::Control::slider); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::slider);
        const core::Control* c = core::grid.control(uid);
        
        layout.add(std::make_unique<juce::AudioParameterFloat> 
        (
            core::grid.name(uid, true), 
            core::grid.name(uid, false), 
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

    for(int i = 0; i < core::grid.count(core::Control::parameter); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::parameter);
        const core::Control* c = core::grid.control(uid);
        
        layout.add(std::make_unique<juce::AudioParameterFloat> 
        (
            core::grid.name(uid, true), 
            core::grid.name(uid, false), 
            c->min,
            c->max,
            c->def
        ));
    }

    for(int i = 0; i < core::grid.count(core::Control::button); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::button);
        layout.add(std::make_unique<juce::AudioParameterBool>
        (
            core::grid.name(uid, true), 
            core::grid.name(uid, false),
            false
        ));
    }

    for(int i = 0; i < core::grid.count(core::Control::input) * core::grid.count(core::Control::output); ++i)
    {
        // auto uid = core::grid.getUID(i, core::Control::input);
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
int Processor::getNumPrograms() { return presets.size(); }
int Processor::getCurrentProgram() { return 0; }
void Processor::setCurrentProgram(int index) {}
const juce::String Processor::getProgramName(int index) { return {}; }
void Processor::changeProgramName(int index, const juce::String& newName) {}

juce::Result Processor::getPresetsFolder()
{
    preset_directory = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory);
    preset_directory = preset_directory.getChildFile("Spiro").getChildFile("Presets");
    juce::Result res = preset_directory.createDirectory();
    std::cout<<"Processor::getPresetsFolder()\n";
    return res;
}

/***************************************************************************************************************************
* 
*  Save preset
* 
**************************************************************************************************************************/
bool Processor::savePreset(juce::String presetName, bool skipIfPresetWithThisNameExists)
{
    suspendProcessing(true);
    getPresetsFolder();

    currentPresetName = presetName;
    juce::MemoryBlock newPresetFileContent;
    getStateInformation(newPresetFileContent);

    auto presetFile = preset_directory.getChildFile(presetName);
    auto tempFile = preset_directory.getChildFile("temp");

    auto file = presetFile.create();

    jassert(file.wasOk());
    if(file.failed())
    {
        suspendProcessing(false);
        return false;
    }
    file = tempFile.create();

    jassert(file.wasOk());
    if(file.failed()) 
    {
        suspendProcessing(false);
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
*  Load preset
* 
**************************************************************************************************************************/
bool Processor::loadPreset(juce::String presetName)
{
    suspendProcessing(true);
    scanPresetDir();
    auto presetFile = findPresetFile(presetName);
    if (presetFile.getFullPathName().isNotEmpty() && presetFile.existsAsFile())
    {
        currentPresetName = presetName;
        juce::MemoryMappedFile file(presetFile, juce::MemoryMappedFile::readOnly);
        setStateInformation(file.getData(), int(file.getSize()));
        suspendProcessing(false);
        return true;
    }

    suspendProcessing(false);
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

const juce::File Processor::findPresetFile(const juce::String& presetNameToLookFor)
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

juce::StringArray Processor::getPresetList()
{
    juce::StringArray presetList;
    presetList.ensureStorageAllocated(int (presets.size()));
    for(auto& preset : presets) presetList.add (preset.first);
    std::cout<<"Processor::getPresetList()\n";
    return presetList;
}

void Processor::scanPresetDir()
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
*  On save
* 
**************************************************************************************************************************/
void Processor::getStateInformation(juce::MemoryBlock& destData)
{
    listeners.call([this](Listener &l) { l.saveCall(); });
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
        }
    }
    listeners.call([this](Listener &l) { l.loadCall(); });
}

void Processor::reset()
{

}

void Processor::reloadParameters()
{
    suspendProcessing(true);
    for(int i = 0; i < core::grid.count(core::Control::slider); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::slider);
        auto raw = tree.getRawParameterValue(core::grid.name(uid, true));
        spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->ccv[uid.pp] = raw;
    }
    
    for(int i = 0; i < core::grid.count(core::Control::parameter); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::parameter);
        auto name = core::grid.name(uid, true);
        auto raw = tree.getRawParameterValue(name);
        spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->ccv[uid.pp] = raw;
        parameters[i] = tree.getParameter(name);
    }

    for(int i = 0; i < core::grid.count(core::Control::input); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::input);
        auto idx = spiro.bay->get_index(core::encode_uid(uid));
        spiro.bay->io[idx].com = &spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->icv[uid.pp];
    }

    for(int i = 0; i < core::grid.count(core::Control::output); ++i)
    {
        auto uid = core::grid.getUID(i, core::Control::output);
        auto idx = spiro.bay->get_index(core::encode_uid(uid));
        spiro.bay->io[idx].data = &spiro.rack.at(static_cast<core::map::module::type>(uid.mt), uid.mp)->ocv[uid.pp];
    }

    for(int i = 0; i < core::grid.count(core::Control::input) * core::grid.count(core::Control::output); ++i)
    {
        matrix[i] = tree.getParameter("mm" + juce::String(i));
    }

    suspendProcessing(false);
}



/***************************************************************************************************************************
* 
*  Prepare to play 
* 
**************************************************************************************************************************/
void Processor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    std::cout<<"-- Processor: prepareToPlay()\n";

    core::settings::buffer_size = samplesPerBlock;
    core::settings::sample_rate = sampleRate;
    core::settings::reset_time_multiplier();
    std::cout<<"Samples per block : "<<samplesPerBlock<<"\n";
    std::cout<<"Sample rate       : "<<sampleRate<<"\n";
    buffer = std::make_shared<core::wavering<core::Point2D<float>>>(samplesPerBlock * core::settings::scope_fps);
    if(getActiveEditor())
    {
        listeners.call([this](Listener &l) { l.resetCall(); });
    }
    reloadParameters();
    suspendProcessing(false);
}

void Processor::releaseResources()
{
    // suspendProcessing(true);
}

/******************************************************************************************************************************
* 
*  MIDI
* 
******************************************************************************************************************************/
void Processor::handleMIDI(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for(const auto metadata : midiMessages) 
    {
        uint8_t status  = metadata.data[0];
        uint8_t msb = (metadata.numBytes >= 2) ? metadata.data[1] : 0;
        uint8_t lsb = (metadata.numBytes == 3) ? metadata.data[2] : 0;
        spiro.midi_message(status, msb, lsb);
    }
    midiMessages.clear();
}

/******************************************************************************************************************************
* 
*  Processing
* 
******************************************************************************************************************************/
void Processor::processBlock(juce::AudioBuffer<float>& data, juce::MidiBuffer& midiMessages)
{
	handleMIDI(data, midiMessages);
	data.clear();
	int samples = data.getNumSamples();
	float* DataL = data.getWritePointer(0);
	float* DataR = data.getWritePointer(1);

	for(int i = 0; i < samples; i++)
	{
	    spiro.process();
	    auto L = spiro.out[core::Spiro::stereo::l].load();
	    auto R = spiro.out[core::Spiro::stereo::r].load();
		DataL[i] = L * 0.2f;
		DataR[i] = R * 0.2f;
		buffer.get()->set(core::Point2D<float>{ L , R });
	}
}


juce::AudioProcessorEditor* Processor::createEditor()
{
    std::cout<<"-- Processor: createEditor()\n";

    suspendProcessing(true);
    return new Editor(*this, tree);
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Processor();
}
