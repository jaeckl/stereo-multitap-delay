#include "StereoDelayLineConfig.h"
#include "Audio/DelayTapsModel.h"

void StereoDelayLineConfig::init(int delayLineSize) {
  delayLineLeft.setBufferSize(delayLineSize);
  delayLineRight.setBufferSize(delayLineSize);
}

void StereoDelayLineConfig::clear() {
  delayLineLeft.clear();
  delayLineRight.clear();
}

void StereoDelayLineConfig::processBlock(juce::AudioBuffer<float> &buffer) {
  auto *inBufferLeft = buffer.getReadPointer(CHANNEL_LEFT);
  auto *inBufferRight = buffer.getReadPointer(CHANNEL_RIGHT);

  auto *outBufferLeft = buffer.getWritePointer(CHANNEL_LEFT);
  auto *outBufferRight = buffer.getWritePointer(CHANNEL_RIGHT);
  for (auto sampleIndex = 0; sampleIndex < buffer.getNumSamples();
       ++sampleIndex) {
    outBufferLeft[sampleIndex] =
        delayLineLeft.processSample(inBufferLeft[sampleIndex]);
    outBufferRight[sampleIndex] =
        delayLineRight.processSample(inBufferRight[sampleIndex]);
  }
}

void StereoDelayLineConfig::setPointModel(int channel, XYPointModel *model) {
  if (channel == CHANNEL_LEFT)
    delayLineLeft.setDelayModel(
        DelayTapsModel::fromPointModel(delayLineLeft.getBufferSize(), model));
  if (channel == CHANNEL_RIGHT)
    delayLineRight.setDelayModel(
        DelayTapsModel::fromPointModel(delayLineRight.getBufferSize(), model));
}
