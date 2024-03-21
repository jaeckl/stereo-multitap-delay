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
#include <juce_audio_basics/juce_audio_basics.h>
class IDelayLineConfig {
public:
  virtual ~IDelayLineConfig() = default;
  virtual void init(int delayLineSize) = 0;
  virtual void setDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) = 0;
  virtual void insertDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) = 0;
  virtual void
  addDelayLineReader(int channelIndex, int delayInSamples, float value) = 0;
  virtual void removeDelayLineReader(int channelIndex, int delayIndex) = 0;
  virtual void clear() = 0;
  virtual void processBlock(juce::AudioBuffer<float> &buffer) = 0;
};
