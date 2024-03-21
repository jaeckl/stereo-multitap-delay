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
#include "XYPointModel.h"
#include <gsl/gsl_spline.h>
#include <juce_gui_basics/juce_gui_basics.h>
class XYPointCanvas : public juce::Component {
public:
  enum PathStyle { Linear, Cubic };
  enum Mode { Points, Curve };
  explicit XYPointCanvas(const juce::String &name);
  XYPointCanvas();

  void setPointModel(XYPointModel *pointModel);
  void setDrawMode(Mode drawMode);
  void setPathStyle(PathStyle style);
  void mouseDown(const juce::MouseEvent &event) override;

  void mouseUp(const juce::MouseEvent &event) override;

  void mouseDrag(const juce::MouseEvent &event) override;

  void mouseDoubleClick(const juce::MouseEvent &event) override;
  void modifierKeysChanged(const juce::ModifierKeys &keys) override;

  void setColour(juce::Colour colour);

  void paint(juce::Graphics &g) override;

private:
  XYPointModel *model;
  bool isDragging;
  bool isControlDown;
  Mode mode;
  PathStyle pathStyle;
  gsl_spline *spline;
  std::vector<double> spline_x;
  std::vector<double> spline_y;
  juce::Colour pointColour;
  std::optional<size_t> optionalPointIndex;
  void reset_gsl_spline();
  void resetPointVertical(int pointIndex, const juce::Point<int> &position);
  void resetPointUnconstraint(int pointIndex, const juce::Point<int> &position);
  void resetPointConstraint(int pointIndex, const juce::Point<int> &position);
  bool isInnerPoint(int pointIndex);
  void updateSelectedPointFrom(const juce::Point<int> mousePosition);
  void drawCurve(juce::Graphics &g);
};
