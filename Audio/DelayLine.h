#pragma once

#include "DelayTapsModel.h"
#include "IDelayLine.h"
#include "Mixer/LinearMixer.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/system/juce_PlatformDefs.h>
#include <memory>

template <typename SampleType> class DelayLine : public IDelayLine<SampleType> {
public:
  DelayLine()
      : DelayLine(0, nullptr) {}

  DelayLine(int bufferSize, DelayTapsModel *model)
      : buffer(1, bufferSize)
      , model(model)
      , mixer(1.0f, 1.0f)
      , writeIndex(0)
      , feedBack(0.0) {}

  SampleType processSample(SampleType sample) override {

    SampleType outputSample = 0;
    if (model) {
      for (int i = 0; i < model->getNumDelayTaps(); i++) {
        DelayTapsModel::DelayTapInfo info = model->getDelayTap(i);
        jassert(juce::isPositiveAndBelow(
            info.delayInSamples, buffer.getNumSamples() - 2));
        int readIndex = writeIndex - info.delayInSamples - 1;

        if (readIndex < 0)
          readIndex = buffer.getNumSamples() + readIndex;

        float delaySample = buffer.getSample(0, readIndex);

        mixer.beta(info.amplitude);
        outputSample = mixer.mix(outputSample, delaySample);
      }
    }
    buffer.setSample(0, writeIndex, sample + feedBack * outputSample);

    if (++writeIndex >= buffer.getNumSamples())
      writeIndex = 0;
    return outputSample;
  }

  void clear() override { buffer.clear(); }

  void setBufferSize(int bufferSize) override {
    buffer.setSize(1, bufferSize + 1);
    writeIndex = 0;
  }

  int getBufferSize() override { return buffer.getNumSamples(); }

  void setFeedBack(float feedBackAmount) override { feedBack = feedBackAmount; }
  void setDelayModel(DelayTapsModel *newModel) override {
    model.reset(newModel);
  }

private:
  juce::AudioBuffer<SampleType> buffer;
  std::unique_ptr<DelayTapsModel> model;
  LinearMixer<float> mixer;
  int writeIndex;
  float feedBack;
};
