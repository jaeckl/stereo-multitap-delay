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
  virtual void addPoint(const juce::Point<float> &point) = 0;
  virtual void insertPoint(int index, const juce::Point<float> &point) = 0;
  virtual void removePoint(int pointIndex) = 0;
  virtual juce::Point<float> getPoint(int pointIndex) = 0;
  virtual void clearPoints() = 0;
  virtual int getNumPoints() = 0;
  virtual void resetPoint(int pointIndex, const juce::Point<float> &point) = 0;
  virtual std::vector<juce::Point<float>> getPoints() = 0;
  virtual void addListener(Listener *listener) = 0;
  virtual void removeListener(Listener *listener) = 0;
};
