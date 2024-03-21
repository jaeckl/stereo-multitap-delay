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
#include "Components/XYPointModel.h"
#include "PluginProcessor.h"
#include <memory>
class IDelayEditorConfig : public XYPointModel {
public:
  virtual void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      AudioPluginAudioProcessor *processor) = 0;
  virtual ~IDelayEditorConfig() = default;
};
