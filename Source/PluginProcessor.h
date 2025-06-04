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
#include <vector>
#include "Socket.h"
#include "wavering.hpp"
#include "EnvelopeDisplay.h"
#include "spiro.hpp"

class Processor: public juce::AudioProcessor
{
    public:
        juce::AudioProcessorEditor* createEditor() override;
        juce::AudioProcessorValueTreeState tree;
        juce::RangedAudioParameter** parameters;
        juce::AudioProcessorParameter** matrix;
        
        juce::CriticalSection localResourcesLock;
        juce::CriticalSection parametersLock;
        juce::CriticalSection sharedResourcesLock;

        juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;
        void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
        void setCurrentProgram(int index) override;
        void changeProgramName(int index, const juce::String& newName) override;
        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;

        double getTailLengthSeconds() const override { return 0.0; };
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override { return true; };
        bool hasEditor() const override { return true; }
        bool acceptsMidi() const override { return true; };
        bool producesMidi() const override { return false; };
        bool isMidiEffect() const override { return false; };
        int  getNumPrograms() override;
        int  getCurrentProgram() override;

       /**********************************************************************************************************************
        * 
        *  Presets
        *
        **********************************************************************************************************************/
        std::vector<std::pair<juce::String, const juce::File>> presets;
        std::pair<juce::String, const juce::File> current_preset;
        juce::String currentPresetName;
        bool currentPresetWasModified = false;
        bool presetLoadingInProgress = false;
        juce::Identifier presetNameID {"PresetName"};
        
        int n_presets;
        juce::File preset_directory;
        juce::Array<juce::File> presets_available;

        juce::StringArray getPresetList();
        void scanPresetDir();
        const juce::File findPresetFile(const juce::String&);
        void presetFilesAvailableChanged();
        juce::Result getPresetsFolder();
        void saveMatrix();
        bool savePreset(juce::String, bool);
        bool loadPreset(juce::String);
        void reset();
        void reloadParameters();

        const juce::String getName() const override { return JucePlugin_Name; };
        const juce::String getProgramName (int index) override;
        void handleMIDI(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

        juce::AudioDeviceManager deviceManager;
        core::Spiro spiro;
        std::unique_ptr<Sockets> sockets;

        std::shared_ptr<core::wavering<core::Point2D<float>>> buffer;
        
        bool armed = false;

        class Listener 
        {
            public:
                virtual ~Listener() = default;
                virtual void saveCall() {};
                virtual void loadCall() {};
                virtual void resetCall() {};
        };
    
        void addListener(Listener *l)       { listeners.add(l);     }
        void removeListener(Listener *l)    { listeners.remove(l);  }

        Processor();
       ~Processor() override;

    private:
        juce::ListenerList<Listener> listeners;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
