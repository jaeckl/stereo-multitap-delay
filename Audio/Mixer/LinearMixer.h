#pragma once
#include "ISampleMixer.h"
template <typename SampleType>
class LinearMixer : public ISampleMixer<SampleType> {
public:
  LinearMixer(float alpha, float beta)
      : m_alpha(alpha)
      , m_beta(beta) {}
  SampleType mix(SampleType a, SampleType b) override {
    return m_alpha * a + m_beta * b;
  }
  void alpha(float alpha) { m_alpha = alpha; }
  float alpha() { return m_alpha; }
  void beta(float beta) { m_beta = beta; }
  float beta() { return m_beta; }

private:
  float m_alpha, m_beta;
};
