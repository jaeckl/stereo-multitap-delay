#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <limits>
#include <vector>

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
  virtual void removePoint(int pointIndex) = 0;
  virtual juce::Point<float> getPoint(int pointIndex) = 0;
  virtual void clearPoints() = 0;
  virtual int getNumPoints() = 0;
  virtual void resetPoint(int pointIndex, const juce::Point<float> &point) = 0;
  virtual std::vector<juce::Point<float>> getPoints() = 0;
  virtual void addListener(Listener *listener) = 0;
  virtual void removeListener(Listener *listener) = 0;
};

class XYPointCanvas : public juce::Component {
public:
  explicit XYPointCanvas(const juce::String &name)
      : XYPointCanvas() {
    setName(name);
  }
  XYPointCanvas()
      : model()
      , isDragging(false)
      , isControlDown(false)
      , optionalPointIndex() {
    setMouseClickGrabsKeyboardFocus(false);
  }

  void setPointModel(XYPointModel *pointModel) { model = pointModel; }

  void mouseDown(const juce::MouseEvent &event) override {
    isDragging = true;
    updateSelectedPointFrom(event.getMouseDownPosition());
    repaint();
  }

  void mouseUp(const juce::MouseEvent &event) override {
    juce::ignoreUnused(event);
    isDragging = false;
    optionalPointIndex.reset();
    repaint();
  }

  void mouseDrag(const juce::MouseEvent &event) override {
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

  void mouseDoubleClick(const juce::MouseEvent &event) override {
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
  void modifierKeysChanged(const juce::ModifierKeys &keys) override {
    if (keys.isCtrlDown()) {
      isControlDown = true;
    } else {
      isControlDown = false;
    }
  }

  void setColour(juce::Colour colour) { pointColour = colour; }

  void paint(juce::Graphics &g) override {
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

private:
  XYPointModel *model;
  bool isDragging;
  bool isControlDown;
  std::optional<size_t> optionalPointIndex;

  juce::Colour pointColour = juce::Colour(76, 144, 255);

  void updateSelectedPointFrom(const juce::Point<int> mousePosition) {
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
};
