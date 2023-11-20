// /*
//  MIT License

//  Copyright (c) 2020 Janos Buttgereit

//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:

//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.

//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//  */

// namespace jb
// {
// class PresetManagerComponent;

// class StateAndPresetManager : private juce::AudioProcessorValueTreeState::Listener
// {
// public:
//     StateAndPresetManager (juce::AudioProcessor& processorToCotrol,
//                            juce::AudioProcessorValueTreeState& apvts,
//                            juce::StringArray&& managedParameters,
//                            juce::UndoManager& undoManager);

//     ~StateAndPresetManager() override;

//     std::unique_ptr<PresetManagerComponent> createPresetManagerComponent (juce::Component& editor, bool withUndoRedoButtons);

//     bool loadPreset (const juce::String& presetName);

//     void storePreset (const juce::String& presetName, bool skipIfPresetWithThisNameExists = false);

//     void getStateInformation (juce::MemoryBlock& destData);
//     void setStateInformation (const void* data, int sizeInBytes);

//     static const juce::File presetDirectory;
// private:
//     friend class PresetManagerComponent;

//     template <class ParameterProvider>
//     friend class PluginAudioProcessorBase;

//     static const juce::Identifier presetNameID;

//     static juce::Array<juce::File>             presetFilesAvailable;
//     static juce::Array<StateAndPresetManager*> allManagers;
//     static juce::CriticalSection               sharedResourcesLock;

//     juce::AudioProcessor&               processor;
//     juce::UndoManager&                  undoManager;
//     juce::AudioProcessorValueTreeState& parameters;
//     juce::CriticalSection               parametersLock;
//     bool                                presetLoadingInProgress = false;

//     using NameFileMapping = std::pair<juce::String, const juce::File>;
//     std::vector<NameFileMapping> presets;
//     juce::String                 currentPresetName;
//     bool                         currentPresetWasModified = false;
//     juce::CriticalSection        localResourcesLock;

//     PresetManagerComponent* presetManagerComponent = nullptr;

//     const juce::File findPresetFile (const juce::String& presetNameToLookFor);

//     void presetFilesAvailableChanged();

//     juce::StringArray getPresetList();

//     void parameterChanged (const juce::String &parameterID, float /* newValue */) override;
//     void modifiedCurrentPreset();

//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateAndPresetManager)
// };

// class PresetManagerComponent : public juce::Component
// {
// public:
//     ~PresetManagerComponent() override;

//     using OnResizeFunction = std::function<void(const Component& presetManagerComponent)>;

//     void paint (juce::Graphics& g) override;

//     /** This lambda can be used to override the default layout of the sub-components */
//     OnResizeFunction onResizeFunction = nullptr;

//     juce::Component& getSaveButton() { return saveButton; }
//     juce::Component& getPresetMenu() { return presetMenu; }

//     juce::Component* getUndoButton() { return undoButton.get(); }
//     juce::Component* getRedoButton() { return redoButton.get(); }

//     const bool hasRedoUndoButtons;
// private:

//     struct SaveComponent : public juce::Component
//     {
//         SaveComponent (PresetManagerComponent* parent);

//         void paint (juce::Graphics& g) override;

//         void resized() override;

//         SafePointer<PresetManagerComponent> presetManagerComponent;
//         juce::TextEditor editor;
//         juce::TextButton saveButton   { "Ok" };
//         juce::TextButton cancelButton { "Cancel" };
//     };

//     friend class StateAndPresetManager;

//     juce::Component&       editor;
//     StateAndPresetManager& manager;

//     juce::TextButton saveButton { "Save Preset" };
//     juce::ComboBox   presetMenu;
//     std::unique_ptr<juce::TextButton> undoButton, redoButton;

//     SafePointer<SaveComponent> saveComponent;

//     PresetManagerComponent (juce::Component& editorToOverlayWithSaveDialogue, StateAndPresetManager* managerToReferTo, bool withRedoUndoButtons);

//     void resized() override;

//     void presetsAvailableChanged();
//     void modifiedCurrentPreset();

//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManagerComponent)
// };

// }