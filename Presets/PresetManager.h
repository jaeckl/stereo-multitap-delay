#pragma once
#include "IPreset.h"
#include "vector"
#include <memory>
class PresetManager {
public:
  PresetManager();
  const IPreset *getPreset(size_t presetId);
  juce::StringArray getPresetNameList();

  size_t loadPresetFromFile(const juce::File &file);

private:
  std::vector<std::unique_ptr<IPreset>> presets;
  void loadFromMemory();
  void loadFromFileSystem();
};
