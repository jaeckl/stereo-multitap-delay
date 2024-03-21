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
