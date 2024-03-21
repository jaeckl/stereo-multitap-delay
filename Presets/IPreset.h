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
#include <juce_graphics/juce_graphics.h>
#include <vector>
class IPreset {
public:
  ~IPreset() = default;
  virtual void initialize(double sampleRate, double bpm) = 0;
  virtual const std::vector<juce::Point<float>> &getPresetPoints() const = 0;
  virtual juce::String getName() = 0;
};
