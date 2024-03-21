/*
Copyright (C) 2024  Ludwig Jäck

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

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

size_t PresetManager::loadPresetFromFile(const juce::File &file) {
  presets.push_back(FilePreset::fromFile(file));
  return presets.size() - 1;
}
