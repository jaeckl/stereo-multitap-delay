#pragma once
template <typename SampleType> class IDelayLine {
public:
  ~IDelayLine() = default;
  virtual SampleType processSample(SampleType sample) = 0;
  virtual int addReadingHead(int delayInSamples, float mixingAmplitude) = 0;
  virtual void
  setReadingHead(int delayIndex, int delayInSamples, float amplitude) = 0;
  virtual void
  insertReadingHead(int delayIndex, int delayInSamples, float amplitude) = 0;

  virtual void removeReadingHead(int delayIndex) = 0;
  virtual void clear() = 0;
  virtual void setBufferSize(int bufferSize) = 0;
  virtual int getBufferSize() = 0;
  virtual void setFeedBack(float feedbackAmout) = 0;
};
struct HeadInfo {
  int delayInSamples;
  float mixingAmplitude;
};
