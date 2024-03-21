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
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
class LowpassButton : public juce::DrawableButton {
public:
  explicit LowpassButton(const juce::String &name);

private:
  std::unique_ptr<juce::Drawable> normalImage;
  std::unique_ptr<juce::Drawable> overImage;
  std::unique_ptr<juce::Drawable> normalImageOn;
  std::unique_ptr<juce::Drawable> overImageOn;
};
