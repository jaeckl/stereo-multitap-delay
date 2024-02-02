#pragma once

#include "DelayLineConfig.h"
#include "PluginProcessor.h"
#include "XYPointCanvas.h"
#include <algorithm>
#include <exception>
#include <memory>
#include <vector>
class IDelayEditorConfig : public XYPointModel {
public:
  virtual void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      AudioPluginAudioProcessor *processor) = 0;
  virtual ~IDelayEditorConfig() = default;
};
// for combined and mono
class SingleDelayEditorConfig : public IDelayEditorConfig {
private:
  std::shared_ptr<IDelayLineConfig> delayLineConfig;
  AudioPluginAudioProcessor *processorRef;
  std::vector<juce::Point<float>> points;
  std::vector<XYPointModel::Listener *> listeners;
  bool isStereoDelay = false;

public:
  void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      AudioPluginAudioProcessor *processor) override {
    processorRef = processor;
    if (auto conf = std::dynamic_pointer_cast<MonoDelayLineConfig>(config)) {
      delayLineConfig = conf;
      isStereoDelay = false;
      delayLineConfig->clear();
      for (auto &point : points) {
        int delayInSamples =
            static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
        delayLineConfig->addDelayLineReader(
            0, delayInSamples, 1.0f - point.getY());
      }

      return;
    }
    if (auto conf = std::dynamic_pointer_cast<StereoDelayLineConfig>(config)) {
      delayLineConfig = conf;
      isStereoDelay = true;
      delayLineConfig->clear();
      for (auto &point : points) {
        int delayInSamples =
            static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
        delayLineConfig->addDelayLineReader(
            0, delayInSamples, 1.0f - point.getY());
        delayLineConfig->addDelayLineReader(
            1, delayInSamples, 1.0f - point.getY());
      }
      return;
    }
    throw std::exception();
  }

  void addPoint(const juce::Point<float> &point) override {
    points.push_back(point);
    int delayInSamples =
        static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
    if (isStereoDelay) {
      auto config =
          std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
      config->addDelayLineReader(0, delayInSamples, 1.0f - point.getY());
      config->addDelayLineReader(1, delayInSamples, 1.0f - point.getY());
    } else {
      auto config =
          std::dynamic_pointer_cast<MonoDelayLineConfig>(delayLineConfig);
      config->addDelayLineReader(0, delayInSamples, 1.0f - point.getY());
    }
  }

  void removePoint(int pointIndex) override {
    points.erase(points.begin() + pointIndex);
    if (isStereoDelay) {
      auto config =
          std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
      config->removeDelayLineReader(0, pointIndex);
      config->removeDelayLineReader(1, pointIndex);
    } else {
      auto config =
          std::dynamic_pointer_cast<MonoDelayLineConfig>(delayLineConfig);
      config->removeDelayLineReader(0, pointIndex);
    }
  }
  juce::Point<float> getPoint(int pointIndex) override {
    return points[static_cast<size_t>(pointIndex)];
  }
  void clearPoints() override { points.clear(); }

  int getNumPoints() override { return points.size(); }

  void resetPoint(int pointIndex, const juce::Point<float> &point) override {
    points[static_cast<size_t>(pointIndex)] = point;
    int delayInSamples =
        static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
    if (isStereoDelay) {
      auto config =
          std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
      config->setDelayLineValue(
          0, pointIndex, delayInSamples, 1.0f - point.getY());
      config->setDelayLineValue(
          1, pointIndex, delayInSamples, 1.0f - point.getY());
    } else {
      auto config =
          std::dynamic_pointer_cast<MonoDelayLineConfig>(delayLineConfig);
      config->setDelayLineValue(
          0, pointIndex, delayInSamples, 1.0f - point.getY());
    }
  }
  std::vector<juce::Point<float>> getPoints() override { return points; }

  void addListener(Listener *listener) override {
    listeners.push_back(listener);
  }

  void removeListener(Listener *listener) override {
    auto it = std::find(listeners.begin(), listeners.end(), listener);
    if (it != listeners.end())
      listeners.erase(it);
  }
};
// for stereo
class MultiDelayEditorConfig : public IDelayEditorConfig {
private:
  std::shared_ptr<IDelayLineConfig> delayLineConfig;
  AudioPluginAudioProcessor *processorRef;
  bool isGroupAEnabled = false;
  std::vector<juce::Point<float>> pointsLeft;
  std::vector<juce::Point<float>> pointsRight;
  std::vector<XYPointModel::Listener *> listeners;

public:
  void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      AudioPluginAudioProcessor *processor) override {
    processorRef = processor;
    if (auto conf = std::dynamic_pointer_cast<StereoDelayLineConfig>(config)) {
      delayLineConfig = conf;
      delayLineConfig->clear();
      for (auto &point : pointsLeft) {
        int delayInSamples =
            static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
        delayLineConfig->addDelayLineReader(
            0, delayInSamples, 1.0f - point.getY());
      }
      for (auto &point : pointsRight) {
        int delayInSamples =
            static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
        delayLineConfig->addDelayLineReader(
            1, delayInSamples, 1.0f - point.getY());
      }
      return;
    }
    throw std::exception();
  }

  void enableGroupA() { isGroupAEnabled = true; }
  void enableGroupB() { isGroupAEnabled = false; }

  void addPoint(const juce::Point<float> &point) override {
    int delayInSamples =
        static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
    auto config =
        std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
    if (isGroupAEnabled) {
      pointsLeft.push_back(point);
      config->addDelayLineReader(0, delayInSamples, 1.0f - point.getY());
    } else {
      pointsRight.push_back(point);
      config->addDelayLineReader(1, delayInSamples, 1.0f - point.getY());
    }
  }
  void removePoint(int pointIndex) override {
    auto config =
        std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
    if (isGroupAEnabled) {
      pointsLeft.erase(pointsLeft.begin() + pointIndex);
      config->removeDelayLineReader(0, pointIndex);
    } else {
      pointsRight.erase(pointsRight.begin() + pointIndex);
      config->removeDelayLineReader(1, pointIndex);
    }
  }
  juce::Point<float> getPoint(int pointIndex) override {
    if (isGroupAEnabled) {
      return pointsLeft[static_cast<size_t>(pointIndex)];
    } else {
      return pointsRight[static_cast<size_t>(pointIndex)];
    }
  }
  void clearPoints() override {
    if (isGroupAEnabled) {
      pointsLeft.clear();
    } else {
      pointsRight.clear();
    }
  }
  int getNumPoints() override {
    if (isGroupAEnabled) {
      return pointsLeft.size();
    } else {
      return pointsRight.size();
    }
  }
  void resetPoint(int pointIndex, const juce::Point<float> &point) override {
    int delayInSamples =
        static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
    auto config =
        std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
    if (isGroupAEnabled) {
      pointsLeft[static_cast<size_t>(pointIndex)] = point;
      config->setDelayLineValue(
          0, pointIndex, delayInSamples, 1.0f - point.getY());
    } else {
      pointsRight[static_cast<size_t>(pointIndex)] = point;
      config->setDelayLineValue(
          1, pointIndex, delayInSamples, 1.0f - point.getY());
    }
  }
  std::vector<juce::Point<float>> getPoints() override {
    if (isGroupAEnabled) {
      return pointsLeft;
    } else {
      return pointsRight;
    }
  }
  void addListener(Listener *listener) override {
    listeners.push_back(listener);
  }
  void removeListener(Listener *listener) override {
    auto it = std::find(listeners.begin(), listeners.end(), listener);
    if (it != listeners.end())
      listeners.erase(it);
  }
};
