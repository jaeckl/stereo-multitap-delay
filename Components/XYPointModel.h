#pragma once
#include <juce_graphics/juce_graphics.h>
#include <vector>
class XYPointModel {
public:
  class Listener {
  public:
    ~Listener() = default;
    virtual void pointAdded(juce::Point<float> point) = 0;
    virtual void pointRemoved(juce::Point<float> point) = 0;
    virtual void pointReseted(juce::Point<float> point) = 0;
  };
  ~XYPointModel() = default;
  void addPoint(const juce::Point<float> &point);
  void insertPoint(int index, const juce::Point<float> &point);
  void removePoint(int pointIndex);
  juce::Point<float> getPoint(int pointIndex);
  void clearPoints();
  int getNumPoints();
  void resetPoint(int pointIndex, const juce::Point<float> &point);
  std::vector<juce::Point<float>> getPoints();
  void addListener(Listener *listener);
  void removeListener(Listener *listener);

private:
  std::vector<juce::Point<float>> data;
  std::vector<Listener *> listeners;
};
