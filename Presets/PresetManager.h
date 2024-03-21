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
