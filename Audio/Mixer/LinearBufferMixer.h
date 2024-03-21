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

#include "IBufferMixer.h"
#include <juce_audio_basics/juce_audio_basics.h>
template <typename SampleType>
class LinearBufferMixer : private IBufferMixer<SampleType> {
private:
  float wetLevel;

public:
  void setWetLevel(float level) { wetLevel = level; }
  float getWetLevel() { return wetLevel; }
  void
  mix(juce::AudioBuffer<SampleType> output,
      juce::AudioBuffer<SampleType> a,
      juce::AudioBuffer<SampleType> b) override {
    for (auto channel = 0; channel < output.getNumChannels(); ++channel) {
      auto *writeBuffer = output.getWritePointer(channel);
      auto *readBufferA = a.getReadPointer(channel);
      auto *readBufferB = b.getReadPointer(channel);
      for (auto sample = 0; sample < output.getNumSamples(); ++sample) {
        writeBuffer[sample] = (1 - wetLevel) * readBufferA[sample] +
                              wetLevel * readBufferB[sample];
      }
    }
  }
};
