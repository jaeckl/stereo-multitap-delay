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
#include "DelayLine.h"
#include "IDelayLineConfig.h"
class MonoDelayLineConfig : public IDelayLineConfig {
private:
  const int CHANNEL_MONO = 0;
  DelayLine<float> delayLine;

public:
  void init(int delayLineSize) override;

  void clear() override;

  void processBlock(juce::AudioBuffer<float> &buffer) override;
  void addDelayLineReader(int channelIndex, int delayInSamples, float value)
      override;
  void setDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) override;
  void insertDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) override;
  void removeDelayLineReader(int channelIndex, int delayIndex) override;
};
