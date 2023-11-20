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

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "cell/feeder.hpp"
#include "Socket.h"
#include "EnvelopeDisplay.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SpiroSynth: public juce::AudioProcessor
{
    public:
        juce::AudioProcessorEditor* createEditor() override;
        juce::AudioProcessorValueTreeState tree;
        juce::RangedAudioParameter* parameter[cell::settings::prm_n];
        juce::AudioProcessorParameter* matrix[cell::settings::ports_in * cell::settings::ports_out];
        
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
        bool isBusesLayoutSupported (const BusesLayout& layouts) const override { return true; };
        bool hasEditor() const override { return true; }
        bool acceptsMidi() const override { return true; };
        bool producesMidi() const override { return false; };
        bool isMidiEffect() const override { return false; };
        int getNumPrograms() override;
        int getCurrentProgram() override;

        // Presets ////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        const juce::File findPresetFile (const juce::String&);
        void presetFilesAvailableChanged();
        juce::Result getPresetsFolder();
        void saveMatrix();
        bool savePreset(juce::String, bool);
        bool loadPreset(juce::String);
        void reset();
        void reloadParameters();

        const juce::String getName() const override { return JucePlugin_Name; };
        const juce::String getProgramName (int index) override;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void handleMIDI(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

        juce::AudioDeviceManager deviceManager;
        cell::feeder feed;
        std::unique_ptr<Sockets> sockets;

        std::unique_ptr<cell::wavering<cell::l_r<float>>> c_buffer;
        
        bool armed = false;

        class Listener 
        {
            public:
                virtual ~Listener() = default;
                virtual void saveCall() {};
                virtual void loadCall() {};

        };
    
        void addListener(Listener *l)       { listeners.add(l);     }
        void removeListener(Listener *l)    { listeners.remove(l);  }

        SpiroSynth();
       ~SpiroSynth() override;

    private:
        juce::ListenerList<Listener> listeners;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpiroSynth)
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
