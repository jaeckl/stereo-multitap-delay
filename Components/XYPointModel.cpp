#include "XYPointModel.h"

void XYPointModel::addPoint(const juce::Point<float> &point) {
  data.push_back(point);
}
void XYPointModel::insertPoint(int index, const juce::Point<float> &point) {
  data.insert(data.begin() + index, point);
}
void XYPointModel::removePoint(int pointIndex) {
  data.erase(data.begin() + pointIndex);
}
juce::Point<float> XYPointModel::getPoint(int pointIndex) {
  return data.at(pointIndex);
}
void XYPointModel::clearPoints() { data.clear(); }
int XYPointModel::getNumPoints() { return data.size(); }
void XYPointModel::resetPoint(int pointIndex, const juce::Point<float> &point) {
  data[pointIndex] = point;
}
std::vector<juce::Point<float>> XYPointModel::getPoints() { return data; }
void XYPointModel::addListener(Listener *listener) {
  listeners.push_back(listener);
}
void XYPointModel::removeListener(Listener *listener) {
  auto itr = std::find(listeners.begin(), listeners.end(), listener);
  if (itr != listeners.end())
    listeners.erase(itr);
}
