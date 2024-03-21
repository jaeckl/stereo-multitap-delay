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

#include "Mixer/LinearMixer.h"
#include "juce_core/system/juce_PlatformDefs.h"

#include "IDelayLine.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

template <typename SampleType> class DelayLine : public IDelayLine<SampleType> {
public:
  DelayLine()
      : DelayLine(100, {}) {}
  DelayLine(int bufferSize)
      : DelayLine(bufferSize, {}) {}
  DelayLine(int bufferSize, std::vector<HeadInfo> readingHeads)
      : m_buffer(1, bufferSize)
      , m_readIndices()
      , m_writeIndex(0)
      , feedBack(0.0) {
    for (auto info : readingHeads)
      addReadingHead(info.delayInSamples, info.mixingAmplitude);
  }

  SampleType processSample(SampleType sample) {

    SampleType outputSample = 0;
    for (size_t i = 0; i < m_readIndices.size(); i++) {
      float delaySample = m_buffer.getSample(0, m_readIndices[i]);
      if (++m_readIndices[i] >= m_buffer.getNumSamples()) {
        m_readIndices[i] = 0;
      }
      outputSample = m_mixers[i].mix(outputSample, delaySample);
    }
    m_buffer.setSample(0, m_writeIndex, sample + feedBack * outputSample);

    if (++m_writeIndex >= m_buffer.getNumSamples())
      m_writeIndex = 0;
    return outputSample;
  }

  void amplitude(int headIndex, float amplitude) {
    if (juce::isPositiveAndNotGreaterThan(headIndex, m_mixers.size())) {
      m_mixers[static_cast<size_t>(headIndex)].beta(amplitude);
    }
  }

  int addReadingHead(int delayInSamples, float mixingAmplitude) {
    jassert(
        juce::isPositiveAndBelow(delayInSamples, m_buffer.getNumSamples() - 1));
    int readIndex = m_writeIndex - delayInSamples - 1;
    if (readIndex < 0)
      readIndex = m_buffer.getNumSamples() + readIndex;
    m_readIndices.push_back(readIndex);
    m_mixers.push_back(LinearMixer<SampleType>(1.0, mixingAmplitude));
    return m_readIndices.size() - 1;
  }

  void insertReadingHead(int delayIndex, int delayInSamples, float amplitude) {
    jassert(juce::isPositiveAndBelow(delayIndex, m_readIndices.size()));
    jassert(
        juce::isPositiveAndBelow(delayInSamples, m_buffer.getNumSamples() - 1));
    int readIndex = m_writeIndex - delayInSamples - 1;
    if (readIndex < 0)
      readIndex = m_buffer.getNumSamples() + readIndex;
    m_readIndices.insert(m_readIndices.begin() + delayIndex, readIndex);
    m_mixers.insert(
        m_mixers.begin() + delayIndex, LinearMixer<SampleType>(1.0, amplitude));

    m_readIndices.insert(m_readIndices.begin() + delayIndex, readIndex);
    m_mixers[static_cast<size_t>(delayIndex)].beta(amplitude);
  }
  void setReadingHead(int delayIndex, int delayInSamples, float amplitude) {
    jassert(juce::isPositiveAndBelow(delayIndex, m_readIndices.size()));
    jassert(
        juce::isPositiveAndBelow(delayInSamples, m_buffer.getNumSamples() - 1));
    int readIndex = m_writeIndex - delayInSamples - 1;
    if (readIndex < 0)
      readIndex = m_buffer.getNumSamples() + readIndex;
    m_readIndices[static_cast<size_t>(delayIndex)] = readIndex;
    m_mixers[static_cast<size_t>(delayIndex)].beta(amplitude);
  }
  void removeReadingHead(int delayIndex) {
    jassert(juce::isPositiveAndBelow(delayIndex, m_readIndices.size()));
    m_readIndices.erase(m_readIndices.begin() + delayIndex);
    m_mixers.erase(m_mixers.begin() + delayIndex);
  }

  void clear() {
    m_readIndices.clear();
    m_mixers.clear();
  }

  void setBufferSize(int bufferSize) {
    m_buffer.setSize(1, bufferSize + 1);
    m_writeIndex = 0;
  }

  int getBufferSize() { return m_buffer.getNumSamples(); }

  void setFeedBack(float feedBackAmount) { feedBack = feedBackAmount; }

private:
  juce::AudioBuffer<SampleType> m_buffer;
  std::vector<int> m_readIndices;
  std::vector<LinearMixer<SampleType>> m_mixers;
  int m_writeIndex;
  float feedBack;
};
