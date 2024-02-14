#include "MonoDelayLineConfig.h"
#include "Audio/DelayTapsModel.h"
void MonoDelayLineConfig::init(int delayLineSize) {
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

void MonoDelayLineConfig::setPointModel(int channel, XYPointModel *model) {
  if (channel == CHANNEL_MONO)
    delayLine.setDelayModel(
        DelayTapsModel::fromPointModel(delayLine.getBufferSize(), model));
}
