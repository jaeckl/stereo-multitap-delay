#pragma once

template <typename SampleType> class SampleMixer {
public:
  ~SampleMixer() = default;
  virtual SampleType mix(SampleType a, SampleType b) = 0;
};
template <typename SampleType>
class LinearCrossfadeMixer : public SampleMixer<SampleType> {
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

template <typename SampleType>
class LinearMixer : public SampleMixer<SampleType> {
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
