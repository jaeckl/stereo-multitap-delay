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
