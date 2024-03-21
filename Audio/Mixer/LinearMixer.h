#pragma once
#include "ISampleMixer.h"
template <typename SampleType>
class LinearMixer : public ISampleMixer<SampleType> {
public:
  LinearMixer(float alpha, float beta)
      : alpha(alpha)
      , beta(beta) {}
  SampleType mix(SampleType a, SampleType b) override {
    return alpha * a + beta * b;
  }
  void setAlpha(float newAlpha) { alpha = newAlpha; }
  float getAlpha() { return alpha; }
  void setBeta(float newBeta) { beta = newBeta; }
  float getBeta() { return beta; }

private:
  float alpha, beta;
};
