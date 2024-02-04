#include "PresetManager.h"
#include "FilePreset.h"
#include <BinaryPresets.h>
PresetManager::PresetManager() {
  loadFromMemory();
  loadFromFileSystem();
}
void PresetManager::loadFromMemory() {
  for (auto i = 0; i < BinaryPresets::namedResourceListSize; ++i) {
    int size = 0;
    const char *data = BinaryPresets::getNamedResource(
        BinaryPresets::namedResourceList[i], size);

    if (data != nullptr && size > 0) {
      presets.push_back(FilePreset::fromString(juce::String(data, size)));
    }
  }
}
void PresetManager::loadFromFileSystem() {}
const IPreset *PresetManager::getPreset(size_t presetId) {
  return presets[presetId].get();
}

juce::StringArray PresetManager::getPresetNameList() {
  juce::StringArray result;
  for (auto &preset : presets)
    result.add(preset->getName());
  return result;
}
