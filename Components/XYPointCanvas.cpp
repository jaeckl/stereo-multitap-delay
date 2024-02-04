#include "XYPointCanvas.h"
#include "XYPointModel.h"
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
    , optionalPointIndex() {
  setMouseClickGrabsKeyboardFocus(false);
}

void XYPointCanvas::setPointModel(XYPointModel *pointModel) {
  model = pointModel;
}

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

    size_t selectedPointIndex = optionalPointIndex.value();

    juce::Point<float> newPoint;
    if (isControlDown) {
      newPoint.setX(model->getPoint(selectedPointIndex).getX());
      newPoint.setY(static_cast<float>(mousePosition.getY()) / getHeight());

    } else {
      newPoint =
          fractionalFromDiscretePoint(mousePosition, getWidth(), getHeight());
    }
    model->resetPoint(selectedPointIndex, newPoint);
  }
  repaint();
}

void XYPointCanvas::mouseDoubleClick(const juce::MouseEvent &event) {
  updateSelectedPointFrom(event.getMouseDownPosition());
  if (optionalPointIndex.has_value()) {
    size_t idx = optionalPointIndex.value();
    model->removePoint(idx);
    optionalPointIndex.reset();
  } else {
    model->addPoint(fractionalFromDiscretePoint(
        event.getPosition(), getWidth(), getHeight()));
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
  g.setColour(pointColour.withAlpha(0.04f));
  g.fillRect(getLocalBounds());

  for (size_t i = 0; i < static_cast<size_t>(model->getNumPoints()); ++i) {
    auto point = discreteFromFractionalPoint(
        model->getPoint(i), getWidth(), getHeight());
    if (optionalPointIndex.has_value() && i == optionalPointIndex.value()) {
      g.setColour(pointColour.withLightness(0.8));
      g.fillEllipse(point.x - 7, point.y - 7, 14, 14);
      g.drawLine(point.x, point.y, point.x, getHeight(), 2);
    } else {
      g.setColour(pointColour);
      g.fillEllipse(point.x - 5, point.y - 5, 10, 10);
      g.drawLine(point.x, point.y, point.x, getHeight(), 2);
    }
  }

  g.setColour(pointColour.withAlpha(0.4f));
  g.drawLine(0, 1, getWidth(), 1, 2);
  g.drawLine(getWidth() - 1, 2, getWidth() - 1, getHeight(), 2);
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
