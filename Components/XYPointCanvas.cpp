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

#include "XYPointCanvas.h"
#include "XYPointModel.h"
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sys.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <limits>

static juce::Point<int> discreteFromFractionalPoint(
    const juce::Point<float> &point,
    int width,
    int height) {
  return {
      static_cast<int>(point.getX() * width),
      static_cast<int>(point.getY() * height)};
}

static juce::Point<float> fractionalFromDiscretePoint(
    const juce::Point<int> &point,
    int width,
    int height) {
  return {
      static_cast<float>(point.getX()) / width,
      static_cast<float>(point.getY()) / height};
}

XYPointCanvas::XYPointCanvas(const juce::String &name)
    : XYPointCanvas() {
  setName(name);
}
XYPointCanvas::XYPointCanvas()
    : model()
    , isDragging(false)
    , isControlDown(false)
    , mode(Points)
    , pathStyle(PathStyle::Linear)
    , pointColour(juce::Colour(76, 144, 255))
    , optionalPointIndex() {
  setMouseClickGrabsKeyboardFocus(false);
}

void XYPointCanvas::setPointModel(XYPointModel *pointModel) {
  model = pointModel;
}

void XYPointCanvas::setDrawMode(Mode drawMode) { mode = drawMode; }
void XYPointCanvas::setPathStyle(PathStyle style) { pathStyle = style; }
void XYPointCanvas::mouseDown(const juce::MouseEvent &event) {
  isDragging = true;
  updateSelectedPointFrom(event.getMouseDownPosition());
  repaint();
}

void XYPointCanvas::mouseUp(const juce::MouseEvent &event) {
  juce::ignoreUnused(event);
  isDragging = false;
  optionalPointIndex.reset();
  repaint();
}

void XYPointCanvas::mouseDrag(const juce::MouseEvent &event) {
  auto mousePosition = event.getPosition();
  if (optionalPointIndex.has_value()) {
    if (!getLocalBounds().contains(mousePosition))
      return;

    size_t pointIndex = optionalPointIndex.value();

    switch (mode) {
    case Points: {
      if (isControlDown)
        resetPointVertical(pointIndex, mousePosition);
      else
        resetPointUnconstraint(pointIndex, mousePosition);
      break;
    }
    case Curve: {
      if (isControlDown)
        resetPointVertical(pointIndex, mousePosition);
      else if (isInnerPoint(pointIndex))
        resetPointConstraint(pointIndex, mousePosition);
      break;
    }
    }
    reset_gsl_spline();
  }
  repaint();
}

void XYPointCanvas::mouseDoubleClick(const juce::MouseEvent &event) {
  updateSelectedPointFrom(event.getMouseDownPosition());
  if (optionalPointIndex.has_value()) {
    size_t pointIndex = optionalPointIndex.value();
    switch (mode) {
    case Points:
      model->removePoint(pointIndex);
      break;
    case Curve: {
      if (isInnerPoint(pointIndex))
        model->removePoint(pointIndex);
      break;
    }
    }

    optionalPointIndex.reset();
    reset_gsl_spline();

  } else {
    size_t index = 0;
    juce::Point<float> newPoint = fractionalFromDiscretePoint(
        event.getPosition(), getWidth(), getHeight());
    for (const auto &point : model->getPoints())
      if (point.getX() < newPoint.getX())
        index++;
    model->insertPoint(index, newPoint);
    reset_gsl_spline();
  }
  repaint();
}
void XYPointCanvas::modifierKeysChanged(const juce::ModifierKeys &keys) {
  if (keys.isCtrlDown()) {
    isControlDown = true;
  } else {
    isControlDown = false;
  }
}

void XYPointCanvas::setColour(juce::Colour colour) { pointColour = colour; }

void XYPointCanvas::paint(juce::Graphics &g) {
  // g.setColour(juce::Colours::red);
  g.setColour(pointColour.withAlpha(0.04f));
  g.fillRect(getLocalBounds());

  if (model) {
    if (mode == Curve && model->getNumPoints() > 1) {
      if (spline == nullptr)
        reset_gsl_spline();
      drawCurve(g);
    }

    for (size_t i = 0; i < static_cast<size_t>(model->getNumPoints()); ++i) {
      auto point = discreteFromFractionalPoint(
          model->getPoint(i), getWidth(), getHeight());
      if (optionalPointIndex.has_value() && i == optionalPointIndex.value()) {
        g.setColour(pointColour.withLightness(0.8));
        g.fillEllipse(point.x - 7, point.y - 7, 14, 14);
        g.drawRect(point.x - 1, point.y, 2, getHeight() - point.y);
      } else {
        g.setColour(pointColour);
        g.fillEllipse(point.x - 5, point.y - 5, 10, 10);
        g.drawRect(point.x - 1, point.y, 2, getHeight() - point.y);
      }
    }
  }

  g.setColour(pointColour.withAlpha(0.4f));
  g.drawLine(0, 1, getWidth(), 1, 2);
  g.drawLine(getWidth() - 1, 2, getWidth() - 1, getHeight(), 2);
}

void XYPointCanvas::drawCurve(juce::Graphics &g) {
  g.setColour(pointColour);

  juce::Path path;
  // used to only draw when y changes to improve smoothness
  float lastYPosFloor = 0.0f;
  auto firstXPos =
      discreteFromFractionalPoint(model->getPoint(0), getWidth(), getHeight())
          .getX();
  auto lastXPos =
      discreteFromFractionalPoint(
          model->getPoint(model->getNumPoints() - 1), getWidth(), getHeight())
          .getX();
  for (auto x = firstXPos; x <= lastXPos; ++x) {
    int y = gsl_spline_eval(spline, static_cast<double>(x), nullptr);
    if (x == 0) {
      path.startNewSubPath(x, y);
      lastYPosFloor = std::floor(y);
    } else {
      if (x == lastXPos || lastYPosFloor != std::floor(y)) {
        path.lineTo(x, y);
        lastYPosFloor = y;
      }
    }
  }
  g.strokePath(path, juce::PathStrokeType(2));
}

void XYPointCanvas::updateSelectedPointFrom(
    const juce::Point<int> mousePosition) {
  const int MAX_CLICK_RADIUS = 5;
  optionalPointIndex.reset();

  if (model->getNumPoints() > 0) {
    int minArgument = 0;
    int minDistance = std::numeric_limits<int>::max();
    for (auto i = 0; i < model->getNumPoints(); ++i) {
      auto point = discreteFromFractionalPoint(
          model->getPoint(i), getWidth(), getHeight());
      auto distance = mousePosition.getDistanceFrom(point);
      if (distance < minDistance) {
        minDistance = distance;
        minArgument = i;
      }
    }

    auto minPoint = discreteFromFractionalPoint(
        model->getPoint(minArgument), getWidth(), getHeight());
    if (minPoint.getDistanceFrom(mousePosition) < MAX_CLICK_RADIUS) {
      optionalPointIndex = minArgument;
    }
  }
}
void XYPointCanvas::resetPointVertical(
    int pointIndex,
    const juce::Point<int> &position) {
  juce::Point<float> result;
  float oldX = model->getPoint(pointIndex).getX();
  result.setX(oldX);
  result.setY(static_cast<float>(position.getY() / getHeight()));
  model->resetPoint(pointIndex, result);
}
void XYPointCanvas::resetPointUnconstraint(
    int pointIndex,
    const juce::Point<int> &position) {
  juce::Point<float> result =
      fractionalFromDiscretePoint(position, getWidth(), getHeight());
  model->resetPoint(pointIndex, result);
}
void XYPointCanvas::resetPointConstraint(
    int pointIndex,
    const juce::Point<int> &position) {
  auto pointLeft = discreteFromFractionalPoint(
      model->getPoint(pointIndex - 1), getWidth(), getHeight());
  auto pointRight = discreteFromFractionalPoint(
      model->getPoint(pointIndex + 1), getWidth(), getHeight());
  if (position.getX() == pointLeft.getX() ||
      position.getX() == pointRight.getX())
    resetPointVertical(pointIndex, position);
  else
    resetPointUnconstraint(pointIndex, position);
}
bool XYPointCanvas::isInnerPoint(int pointIndex) {
  return pointIndex > 0 && pointIndex < model->getNumPoints() - 1;
}
void my_gsl_error_handler(
    const char *reason,
    const char *file,
    int line,
    int gsl_errno) {
  fprintf(stderr, "GSL error: %s:%d: %s (%d)\n", file, line, reason, gsl_errno);
  // Handle the error appropriately - you could clean up resources, inform the
  // user, etc.
}
void XYPointCanvas::reset_gsl_spline() {
  if (spline != nullptr)
    gsl_spline_free(spline);
  gsl_set_error_handler(&my_gsl_error_handler);
  size_t numPoints = static_cast<size_t>(model->getNumPoints());
  spline = gsl_spline_alloc(gsl_interp_cspline, numPoints);
  spline_x.clear();
  spline_y.clear();
  for (auto point : model->getPoints()) {
    auto discretePoint =
        discreteFromFractionalPoint(point, getWidth(), getHeight());
    spline_x.push_back(static_cast<double>(discretePoint.getX()));
    spline_y.push_back(static_cast<double>(discretePoint.getY()));
  }
  gsl_spline_init(spline, spline_x.data(), spline_y.data(), numPoints);
}
