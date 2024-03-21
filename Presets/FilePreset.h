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

#include "IPreset.h"
#include <memory>
#include <vector>
class FilePreset : public IPreset {
public:
  FilePreset(juce::String name, std::vector<juce::Point<float>> points);
  static std::unique_ptr<FilePreset> fromMemory(const char *name, size_t size);
  static std::unique_ptr<FilePreset> fromFile(const juce::File &file);
  static std::unique_ptr<FilePreset> fromString(juce::String xmlString);
  void initialize(double sampleRate, double bpm) override;

  const std::vector<juce::Point<float>> &getPresetPoints() const override;
  juce::String getName() override;

private:
  juce::String presetName;
  std::vector<juce::Point<float>> presetPoints;
};
