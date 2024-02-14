#include "DelayTapsModel.h"
#include "Components/XYPointModel.h"

DelayTapsModel::DelayTapsModel(int maxDelayInSamples, XYPointModel *model)
    : maxDelayInSamples(maxDelayInSamples)
    , model(model) {}

int DelayTapsModel::getNumDelayTaps() { return model->getNumPoints(); }

DelayTapsModel::DelayTapInfo DelayTapsModel::getDelayTap(int tapIndex) {
  juce::Point<float> modelPoint = model->getPoint(tapIndex);
  return {
      static_cast<int>(maxDelayInSamples * modelPoint.getX()),
      1.0f - modelPoint.getY()};
}
DelayTapsModel *
DelayTapsModel::fromPointModel(int maxDelayInSamples, XYPointModel *model) {
  return new DelayTapsModel(maxDelayInSamples, model);
}
