#include "StereoDelayLineConfig.h"

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
void StereoDelayLineConfig::addDelayLineReader(
    int channelIndex,
    int delayInSamples,
    float value) {
  if (channelIndex == CHANNEL_LEFT)
    delayLineLeft.addReadingHead(delayInSamples, value);
  if (channelIndex == CHANNEL_RIGHT)
    delayLineRight.addReadingHead(delayInSamples, value);
}

void StereoDelayLineConfig::setDelayLineValue(
    int channelIndex,
    int delayIndex,
    int delayInSamples,
    float value) {
  if (channelIndex == CHANNEL_LEFT)
    delayLineLeft.setReadingHead(delayIndex, delayInSamples, value);
  if (channelIndex == CHANNEL_RIGHT)
    delayLineRight.setReadingHead(delayIndex, delayInSamples, value);
}

void StereoDelayLineConfig::removeDelayLineReader(
    int channelIndex,
    int delayIndex) {
  if (channelIndex == CHANNEL_LEFT)
    delayLineLeft.removeReadingHead(delayIndex);
  if (channelIndex == CHANNEL_RIGHT)
    delayLineRight.removeReadingHead(delayIndex);
}
