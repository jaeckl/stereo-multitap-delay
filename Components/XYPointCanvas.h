#pragma once
#include "XYPointModel.h"
#include <juce_gui_basics/juce_gui_basics.h>

class XYPointCanvas : public juce::Component {
public:
  explicit XYPointCanvas(const juce::String &name);
  XYPointCanvas();

  void setPointModel(XYPointModel *pointModel);

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
  std::optional<size_t> optionalPointIndex;
  juce::Colour pointColour = juce::Colour(76, 144, 255);
  void updateSelectedPointFrom(const juce::Point<int> mousePosition);
};
