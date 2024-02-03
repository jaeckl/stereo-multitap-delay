#pragma once

template <typename SampleType> class ISampleMixer {
public:
  ~ISampleMixer() = default;
  virtual SampleType mix(SampleType a, SampleType b) = 0;
};
