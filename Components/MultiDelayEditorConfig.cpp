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

#include "MultiDelayEditorConfig.h"
#include "Audio/IDelayLineConfig.h"
#include "Audio/StereoDelayLineConfig.h"
#include "PluginProcessor.h"
#include <exception>
#include <memory>
#include <vector>

void MultiDelayEditorConfig::initialize(
    std::shared_ptr<IDelayLineConfig> config,
    AudioPluginAudioProcessor *processor) {
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

void MultiDelayEditorConfig::enableGroupA() { isGroupAEnabled = true; }
void MultiDelayEditorConfig::enableGroupB() { isGroupAEnabled = false; }

void MultiDelayEditorConfig::addPoint(const juce::Point<float> &point) {
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
void MultiDelayEditorConfig::insertPoint(
    int index,
    const juce::Point<float> &point) {
  if (index == pointsLeft.size()) {
    addPoint(point);
    return;
  }
  int delayInSamples =
      static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
  auto config =
      std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
  if (isGroupAEnabled) {
    pointsLeft.insert(pointsLeft.begin() + index, point);
    config->addDelayLineReader(0, delayInSamples, 1.0f - point.getY());
  } else {
    pointsRight.insert(pointsRight.begin() + index, point);
    config->addDelayLineReader(1, delayInSamples, 1.0f - point.getY());
  }
}
void MultiDelayEditorConfig::removePoint(int pointIndex) {
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
juce::Point<float> MultiDelayEditorConfig::getPoint(int pointIndex) {
  if (isGroupAEnabled) {
    return pointsLeft[static_cast<size_t>(pointIndex)];
  } else {
    return pointsRight[static_cast<size_t>(pointIndex)];
  }
}
void MultiDelayEditorConfig::clearPoints() {
  if (isGroupAEnabled) {
    pointsLeft.clear();
  } else {
    pointsRight.clear();
  }
}
int MultiDelayEditorConfig::getNumPoints() {
  if (isGroupAEnabled) {
    return pointsLeft.size();
  } else {
    return pointsRight.size();
  }
}
void MultiDelayEditorConfig::resetPoint(
    int pointIndex,
    const juce::Point<float> &point) {
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
std::vector<juce::Point<float>> MultiDelayEditorConfig::getPoints() {
  if (isGroupAEnabled) {
    return pointsLeft;
  } else {
    return pointsRight;
  }
}
void MultiDelayEditorConfig::addListener(Listener *listener) {
  listeners.push_back(listener);
}
void MultiDelayEditorConfig::removeListener(Listener *listener) {
  auto it = std::find(listeners.begin(), listeners.end(), listener);
  if (it != listeners.end())
    listeners.erase(it);
}
