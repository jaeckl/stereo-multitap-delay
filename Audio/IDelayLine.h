#pragma once
#include "DelayTapsModel.h"
template <typename SampleType> class IDelayLine {
public:
  ~IDelayLine() = default;
  virtual SampleType processSample(SampleType sample) = 0;
  virtual void clear() = 0;
  virtual void setBufferSize(int bufferSize) = 0;
  virtual int getBufferSize() = 0;
  virtual void setFeedBack(float feedbackAmout) = 0;
  virtual void setDelayModel(DelayTapsModel *model) = 0;
};
