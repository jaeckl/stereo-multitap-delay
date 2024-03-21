/*
Copyright (C) 2024  Ludwig Jäck

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

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
