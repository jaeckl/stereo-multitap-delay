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

#include "MonoDelayLineConfig.h"
void MonoDelayLineConfig::init(int delayLineSize) {
  if (delayLineSize != delayLine.getBufferSize())
    delayLine.setBufferSize(delayLineSize);
}

void MonoDelayLineConfig::clear() { delayLine.clear(); }

void MonoDelayLineConfig::processBlock(juce::AudioBuffer<float> &buffer) {
  auto *inBuffer = buffer.getReadPointer(CHANNEL_MONO);
  auto *outBuffer = buffer.getWritePointer(CHANNEL_MONO);
  for (auto sampleIndex = 0; sampleIndex < buffer.getNumSamples();
       ++sampleIndex) {
    outBuffer[sampleIndex] = delayLine.processSample(inBuffer[sampleIndex]);
  }
}
void MonoDelayLineConfig::addDelayLineReader(
    int channelIndex,
    int delayInSamples,
    float value) {
  juce::ignoreUnused(channelIndex);
  delayLine.addReadingHead(delayInSamples, value);
}

void MonoDelayLineConfig::setDelayLineValue(
    int channelIndex,
    int delayIndex,
    int delayInSamples,
    float value) {
  juce::ignoreUnused(channelIndex);
  delayLine.setReadingHead(delayIndex, delayInSamples, value);
}
void MonoDelayLineConfig::insertDelayLineValue(
    int channelIndex,
    int delayIndex,
    int delayInSamples,
    float value) {
  juce::ignoreUnused(channelIndex);
  delayLine.insertReadingHead(delayIndex, delayInSamples, value);
}

void MonoDelayLineConfig::removeDelayLineReader(
    int channelIndex,
    int delayIndex) {
  juce::ignoreUnused(channelIndex);
  delayLine.removeReadingHead(delayIndex);
}
