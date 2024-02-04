#pragma once

#include "Mixer/LinearMixer.h"
#include "juce_core/system/juce_PlatformDefs.h"

#include "IDelayLine.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

template <typename SampleType>
class ReverseDelayLine : public IDelayLine<SampleType> {
public:
  ReverseDelayLine()
      : ReverseDelayLine(100, {}) {}
  ReverseDelayLine(int bufferSize)
      : ReverseDelayLine(bufferSize, {}) {}
  ReverseDelayLine(int bufferSize, std::vector<HeadInfo> readingHeads)
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
      if (!isRunning[i]) {
        if (m_writeIndex == m_delaysInSamples[i])
          isRunning[i] = true;
        m_readIndices[i] = m_delaysInSamples[i];
      } else {
        float delaySample = m_buffer.getSample(0, m_readIndices[i]);
        if (--m_readIndices[i] < 0) {
          m_readIndices[i] = m_buffer.getNumSamples() - 1;
        }
        outputSample = m_mixers[i].mix(outputSample, delaySample);
        // if (m_readIndices[i] == m_writeIndex)
        //   isRunning[i] = false;
      }
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
    int readIndex = m_writeIndex - delayInSamples;
    if (readIndex < 0)
      readIndex = m_buffer.getNumSamples() + readIndex;
    m_readIndices.push_back(readIndex);
    m_delaysInSamples.push_back(delayInSamples);
    isRunning.push_back(false);
    m_mixers.push_back(LinearMixer<SampleType>(1.0, mixingAmplitude));
    return m_readIndices.size() - 1;
  }

  void setReadingHead(int delayIndex, int delayInSamples, float amplitude) {
    jassert(juce::isPositiveAndBelow(delayIndex, m_readIndices.size()));
    jassert(
        juce::isPositiveAndBelow(delayInSamples, m_buffer.getNumSamples() - 1));
    int readIndex = m_writeIndex - delayInSamples;
    if (readIndex < 0)
      readIndex = m_buffer.getNumSamples() + readIndex;
    m_readIndices[static_cast<size_t>(delayIndex)] = readIndex;
    m_mixers[static_cast<size_t>(delayIndex)].beta(amplitude);
  }

  void removeReadingHead(int delayIndex) {
    jassert(juce::isPositiveAndBelow(delayIndex, m_readIndices.size()));
    m_readIndices.erase(m_readIndices.begin() + delayIndex);
    m_mixers.erase(m_mixers.begin() + delayIndex);
    isRunning.erase(isRunning.begin() + delayIndex);
  }

  void clear() {
    m_readIndices.clear();
    m_mixers.clear();
    isRunning.clear();
  }

  void setBufferSize(int bufferSize) {
    m_buffer.setSize(1, 2 * bufferSize + 1);
    m_writeIndex = 0;
  }

  int getBufferSize() { return m_buffer.getNumSamples() / 2 - 1; }
  void setFeedBack(float feedBackAmount) { feedBack = feedBackAmount; }

private:
  juce::AudioBuffer<SampleType> m_buffer;
  std::vector<bool> isRunning;
  std::vector<int> m_readIndices;
  std::vector<int> m_delaysInSamples;
  std::vector<LinearMixer<SampleType>> m_mixers;
  int m_writeIndex;
  float feedBack;
};
