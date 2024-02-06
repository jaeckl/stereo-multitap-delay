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
