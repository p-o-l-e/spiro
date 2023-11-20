// // #include "../jb_plugin_base.h"

// #ifndef JB_PRESET_FILE_EXTENSION
// #define JB_PRESET_FILE_EXTENSION "xml"
// #endif

// using juce::File;

// namespace jb
// {
// StateAndPresetManager::StateAndPresetManager (juce::AudioProcessor& processorToControl,
//                                               juce::AudioProcessorValueTreeState& apvts,
//                                               juce::StringArray&& managedParameters,
//                                               juce::UndoManager& undoManagerToUse)
//   : processor   (processorToControl),
//     undoManager (undoManagerToUse),
//     parameters  (apvts)
// {
//     if (!presetDirectory.exists())
//         presetDirectory.createDirectory();

//     const auto fileExtensionWildcard = juce::String ("*.") + juce::String (JB_PRESET_FILE_EXTENSION);

//     auto allPresetsFound = presetDirectory.findChildFiles (File::TypesOfFileToFind::findFiles, false, fileExtensionWildcard);

//     {
//         juce::ScopedLock scopedLock (sharedResourcesLock);
//         if (allPresetsFound != presetFilesAvailable)
//         {
//             presetFilesAvailable = allPresetsFound;

//             for (auto* man : allManagers)
//                 man->presetFilesAvailableChanged();
//         }

//         allManagers.add (this);
//         presetFilesAvailableChanged();
//     }

//     for (juce::StringRef p : managedParameters)
//         apvts.addParameterListener (p, this);
// }

// StateAndPresetManager::~StateAndPresetManager()
// {
//     allManagers.removeAllInstancesOf (this);
// }

// std::unique_ptr<PresetManagerComponent> StateAndPresetManager::createPresetManagerComponent (juce::Component& editor, bool withUndoRedoButtons)
// {
//     // Only one preset manager component can be active at the same time
//     jassert (presetManagerComponent == nullptr);

//     presetManagerComponent = new PresetManagerComponent (editor, this, withUndoRedoButtons);

//     return std::unique_ptr<PresetManagerComponent> (presetManagerComponent);
// }

// bool StateAndPresetManager::loadPreset (const juce::String& presetName)
// {
//     juce::ScopedLock localScopedLock (localResourcesLock);

//     auto presetFile = findPresetFile (presetName);
//     if (presetFile.getFullPathName().isNotEmpty() && presetFile.existsAsFile())
//     {
//         currentPresetName = presetName;
//         currentPresetWasModified = false;

//         juce::MemoryMappedFile file (presetFile, juce::MemoryMappedFile::readOnly);

//         juce::ScopedValueSetter<bool> vs (presetLoadingInProgress, true);
//         setStateInformation (file.getData(), int (file.getSize()));
//         return true;
//     }

//     return false;
// }

// void StateAndPresetManager::storePreset (const juce::String& presetName, bool skipIfPresetWithThisNameExists)
// {
//     juce::ScopedLock localScopedLock (localResourcesLock);

//     if (currentPresetWasModified)
//         return;

//     currentPresetName = presetName;

//     juce::MemoryBlock newPresetFileContent;
//     getStateInformation (newPresetFileContent);

//     auto presetFile = findPresetFile (presetName);
//     auto presetFileExisting = presetFile.getFullPathName().isNotEmpty();

//     if (presetFileExisting && skipIfPresetWithThisNameExists)
//         return;

//     if (!presetFileExisting)
//         presetFile = presetDirectory.getChildFile (presetName + "." + JB_PRESET_FILE_EXTENSION);

//     auto creatingFile = presetFile.create();

//     jassert (creatingFile.wasOk());
//     if (creatingFile.failed())
//         return;

//     juce::FileOutputStream stream (presetFile);
//     if (stream.openedOk())
//     {
//         stream.setPosition (0);
//         stream.truncate();
//         stream.write (newPresetFileContent.getData(), newPresetFileContent.getSize());
//     }
//     else
//         return;

//     if (!presetFileExisting)
//     {
//         juce::ScopedLock sharedScopedLock (sharedResourcesLock);
//         presetFilesAvailable.add (std::move (presetFile));
//         presetFilesAvailableChanged();
//     }
// }

// const juce::File StateAndPresetManager::findPresetFile (const juce::String& presetNameToLookFor)
// {
//     auto presetFile = std::find_if (presets.begin(), presets.end(),
//                                     [&presetNameToLookFor] (const NameFileMapping& p)
//                                     {
//                                         return p.first == presetNameToLookFor;
//                                     });

//     if (presetFile != presets.end())
//         return presetFile->second;

//     return {};
// }


// void StateAndPresetManager::presetFilesAvailableChanged()
// {
//     // note: never lock sharedResourcesLock inside here to avoid a deadlock, this function will be called with the
//     // shared resources locked
//     juce::ScopedLock scopedLock (localResourcesLock);

//     presets.clear();

//     for (auto& presetFile : presetFilesAvailable)
//         presets.emplace_back (std::make_pair (presetFile.getFileNameWithoutExtension(), presetFile));

//     if (presetManagerComponent != nullptr)
//         presetManagerComponent->presetsAvailableChanged();
// }

// juce::StringArray StateAndPresetManager::getPresetList()
// {
//     juce::StringArray presetList;
//     presetList.ensureStorageAllocated (int (presets.size()));

//     for (auto& preset : presets)
//         presetList.add (preset.first);

//     return presetList;
// }

// void StateAndPresetManager::getStateInformation (juce::MemoryBlock& destData)
// {
//     parametersLock.enter();
//     auto state = parameters.copyState();
//     parametersLock.exit();

//     state.setProperty (presetNameID, currentPresetWasModified ? "" : currentPresetName, nullptr);

//     auto xml = state.createXml();
//     processor.copyXmlToBinary (*xml, destData);
// }

// void StateAndPresetManager::setStateInformation (const void* data, int sizeInBytes)
// {
//     auto xmlState = processor.getXmlFromBinary (data, sizeInBytes);

//     if (xmlState.get() != nullptr)
//         juce::ScopedLock scopedLock (parametersLock);

//         if (xmlState->hasTagName (parameters.state.getType()))
//         {
//             parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
//             juce::String presetNameLoaded = parameters.state.getProperty (presetNameID, "");

//             if (presetNameLoaded.isNotEmpty())
//                 storePreset (presetNameLoaded, true);
//         }

// }

// void StateAndPresetManager::parameterChanged (const juce::String&, float)
// {
//     if (!presetLoadingInProgress)
//         modifiedCurrentPreset();
// }

// void StateAndPresetManager::modifiedCurrentPreset ()
// {
//     if (currentPresetWasModified || currentPresetName.isEmpty())
//         return;

//     currentPresetWasModified = true;
//     currentPresetName = "***" + currentPresetName;

//     if (presetManagerComponent != nullptr)
//         presetManagerComponent->modifiedCurrentPreset();
// }

// const File StateAndPresetManager::presetDirectory = File::getSpecialLocation (File::SpecialLocationType::userApplicationDataDirectory)
//                                                     #if JUCE_MAC
//                                                     .getChildFile ("Audio/Presets")
//                                                     #endif
//                                                     .getChildFile (JucePlugin_Manufacturer)
//                                                     .getChildFile (JucePlugin_Name);

// const juce::Identifier              StateAndPresetManager::presetNameID ("PresetName");
// juce::Array<juce::File>             StateAndPresetManager::presetFilesAvailable;
// juce::Array<StateAndPresetManager*> StateAndPresetManager::allManagers;
// juce::CriticalSection               StateAndPresetManager::sharedResourcesLock;


// PresetManagerComponent::PresetManagerComponent (juce::Component& editorToOverlayWithSaveDialogue,
//                                                 jb::StateAndPresetManager* managerToReferTo,
//                                                 bool withRedoUndoButtons)
//   : hasRedoUndoButtons (withRedoUndoButtons),
//     editor             (editorToOverlayWithSaveDialogue),
//     manager            (*managerToReferTo)
// {
//     // triggers the preset list to become filled initially
//     presetsAvailableChanged();

//     addAndMakeVisible (saveButton);
//     addAndMakeVisible (presetMenu);

//     saveButton.onClick = [this]()
//     {
//         saveComponent = new SaveComponent (this);
//     };

//     presetMenu.onChange = [this]()
//     {
//         if (presetMenu.getSelectedId() == 0)
//             return;

//         auto presetName = presetMenu.getText();
//         manager.loadPreset (presetName);
//     };

//     if (hasRedoUndoButtons)
//     {
//         undoButton = std::make_unique<juce::TextButton> ("Undo");
//         redoButton = std::make_unique<juce::TextButton> ("Redo");

//         addAndMakeVisible (*undoButton);
//         addAndMakeVisible (*redoButton);

//         undoButton->onClick = [this]() { manager.undoManager.undo(); };
//         redoButton->onClick = [this]() { manager.undoManager.redo(); };
//     }
// }

// void PresetManagerComponent::paint (juce::Graphics& g)
// {
//     g.fillAll (juce::Colours::black);
// }

// void PresetManagerComponent::resized()
// {
//     auto bounds = getLocalBounds();
//     auto buttonWidth = proportionOfWidth (0.2f);

//     if (hasRedoUndoButtons)
//     {
//         undoButton->setBounds (bounds.removeFromRight (buttonWidth).reduced (1));
//         redoButton->setBounds (bounds.removeFromRight (buttonWidth).reduced (1));
//     }

//     saveButton.setBounds (bounds.removeFromRight (buttonWidth).reduced (1));
//     presetMenu.setBounds (bounds.reduced (1));
// }

// PresetManagerComponent::~PresetManagerComponent()
// {
//     // In case a save dialogue is open, remove it
//     if (auto *comp = saveComponent.getComponent())
//         delete comp;

//     // To make sure no one currently talks to the component
//     juce::ScopedLock scopedLock (manager.localResourcesLock);
//     manager.presetManagerComponent = nullptr;
// }

// void PresetManagerComponent::presetsAvailableChanged()
// {
//     auto presetList = manager.getPresetList();

//     presetMenu.clear (juce::dontSendNotification);
//     presetMenu.addItemList (presetList, 1);
//     presetMenu.setText (manager.currentPresetName);
// }

// void PresetManagerComponent::modifiedCurrentPreset()
// {
//     // This has to be executed on the message thread
//     juce::MessageManager::callAsync ([safeThis = SafePointer<PresetManagerComponent> (this)]
//     {
//         if (auto* presetManagerComponent = safeThis.getComponent())
//             presetManagerComponent->presetMenu.setText (presetManagerComponent->manager.currentPresetName);
//     });
// }

// PresetManagerComponent::SaveComponent::SaveComponent (PresetManagerComponent* parent) : presetManagerComponent (parent)
// {
//     editor.onTextChange = [this]()
//     {
//         saveButton.setEnabled (!editor.isEmpty());
//     };

//     saveButton.onClick = [this]()
//     {
//         auto presetName = editor.getText();

//         if (presetName.isEmpty())
//             return;

//         if (auto* comp = presetManagerComponent.getComponent())
//         {
//             juce::ScopedValueSetter<bool> sv (comp->manager.presetLoadingInProgress, true);

//             comp->manager.currentPresetWasModified = false;
//             comp->manager.storePreset (presetName);
//             comp->manager.loadPreset (presetName);

//             comp->presetsAvailableChanged();
//             comp->editor.removeChildComponent (this);
//         }
//         delete this;
//     };

//     cancelButton.onClick = [this]()
//     {
//         if (auto* comp = presetManagerComponent.getComponent())
//             comp->editor.removeChildComponent (this);

//         delete this;
//     };

//     addAndMakeVisible (editor);
//     addAndMakeVisible (saveButton);
//     addAndMakeVisible (cancelButton);

//     setOpaque (true);

//     // Add our component to the editor
//     parent->editor.addAndMakeVisible (this);

//     setBounds (parent->editor.getBounds());

//     // If this is a preset that modified, the three *** have to be removed prior to saving
//     auto nCharsToRemove = parent->manager.currentPresetWasModified ? 3 : 0;

//     editor.setText (parent->presetMenu.getText().substring (nCharsToRemove));
// }

// void PresetManagerComponent::SaveComponent::paint (juce::Graphics& g)
// {
//     g.fillAll (juce::Colours::black.withAlpha (0.9f));
//     g.setColour (juce::Colours::white);

//     auto bounds = getLocalBounds().withTrimmedBottom (proportionOfHeight (0.66f));

//     g.drawText ("Save Preset", bounds, juce::Justification::centred);
// }

// void PresetManagerComponent::SaveComponent::resized()
// {
//     const auto sideTrim = proportionOfWidth (0.05f);
//     auto bounds = getLocalBounds().withTrimmedTop (proportionOfHeight (0.33f))
//                                   .withTrimmedLeft (sideTrim)
//                                   .withTrimmedRight (sideTrim);

//     editor.setBounds (bounds.removeFromTop (proportionOfHeight (0.33f)).withSizeKeepingCentre (bounds.getWidth(), 30));

//     bounds = bounds.withSizeKeepingCentre (bounds.getWidth(), 30);

//     saveButton.setBounds (bounds.removeFromLeft (bounds.proportionOfWidth (0.5f)).withTrimmedRight (sideTrim));
//     cancelButton.setBounds (bounds.withTrimmedLeft (sideTrim));
// }

// }