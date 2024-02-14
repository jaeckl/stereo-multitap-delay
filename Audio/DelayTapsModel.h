#pragma once
#include "Components/XYPointModel.h"
class DelayTapsModel {
private:
  int maxDelayInSamples;
  XYPointModel *model;

public:
  struct DelayTapInfo {
    int delayInSamples;
    float amplitude;
  };
  DelayTapsModel(int maxDelayInSamples, XYPointModel *model);
  int getNumDelayTaps();
  DelayTapInfo getDelayTap(int tapIndex);
  static DelayTapsModel *
  fromPointModel(int maxDelayInSamples, XYPointModel *model);
};
