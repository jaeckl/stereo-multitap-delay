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

#include "Audio/IDelayLineConfig.h"
#include "IDelayEditorConfig.h"
#include "PluginProcessor.h"
#include "XYPointCanvas.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include <vector>

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
      AudioPluginAudioProcessor *processor) override;

  void enableGroupA();
  void enableGroupB();

  void addPoint(const juce::Point<float> &point) override;
  void insertPoint(int index, const juce::Point<float> &point) override;
  void removePoint(int pointIndex) override;
  juce::Point<float> getPoint(int pointIndex) override;
  void clearPoints() override;
  int getNumPoints() override;
  void resetPoint(int pointIndex, const juce::Point<float> &point) override;
  std::vector<juce::Point<float>> getPoints() override;
  void addListener(Listener *listener) override;
  void removeListener(Listener *listener) override;
};
