#pragma once
#include "ISampleMixer.h"
template <typename SampleType>
class LinearCrossfadeMixer : public ISampleMixer<SampleType> {
public:
  explicit LinearCrossfadeMixer(float alpha)
      : m_alpha(alpha) {}
  SampleType mix(SampleType a, SampleType b) override {
    return (1 - m_alpha) * a + m_alpha * b;
  }
  void alpha(float alpha) { m_alpha = alpha; }
  float alpha() { return m_alpha; }

private:
  float m_alpha;
};
