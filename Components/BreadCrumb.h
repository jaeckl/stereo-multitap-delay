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
#include <vector>
class BreadCrumb
    : public juce::Component
    , private juce::Button::Listener {
public:
  class Listener {
  public:
    ~Listener() = default;
    virtual void breadCrumbClicked(const juce::StringArray &array) = 0;
  };

  BreadCrumb(const juce::String &name, juce::LookAndFeel_V4 *lookAndFeel);
  void setCrumbs(juce::StringArray crumbs);
  void buttonClicked(juce::Button *) override;
  void paint(juce::Graphics &) override;
  void resized() override;

  void addListener(Listener *listener);
  void removeListener(Listener *listener);

private:
  std::vector<std::unique_ptr<juce::Component>> components;
  std::vector<juce::String> crumbStrings;
  std::vector<Listener *> listeners;
  juce::LookAndFeel_V4 *look;
  int widthOfCrumbs;
  void addCrumb(juce::String crumb);
  void notifyListeners(const juce::StringArray &array);
};
