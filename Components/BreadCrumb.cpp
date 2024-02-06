#include "BreadCrumb.h"
#include "Constants.h"
#include <algorithm>
#include <iterator>
BreadCrumb::BreadCrumb(
    const juce::String &name,
    juce::LookAndFeel_V4 *lookAndFeel)
    : look(lookAndFeel)
    , widthOfCrumbs(0) {
  setName(name);
}

void BreadCrumb::setCrumbs(juce::StringArray crumbs) {
  removeAllChildren();
  components.clear();
  crumbStrings.clear();
  widthOfCrumbs = 0;

  for (auto crumb : crumbs) {
    addCrumb(crumb);
    crumbStrings.push_back(crumb);
    addCrumb(Constants::TEXT_CRUMB_SYMBOL);
  }
  repaint();
}

void BreadCrumb::addCrumb(juce::String crumb) {
  juce::TextButton *button = new juce::TextButton();
  button->setLookAndFeel(look);
  button->setButtonText(crumb);
  button->addListener(this);
  juce::Font font = look->getTextButtonFont(*button, getHeight());
  int width = font.getStringWidth(crumb);
  if (width < getHeight())
    button->setBounds({widthOfCrumbs, 0, getHeight(), getHeight()});
  else
    button->setBounds(
        {widthOfCrumbs, 0, font.getStringWidth("  ") + width, getHeight()});

  button->setColour(
      juce::TextButton::buttonNormal, findColour(juce::Button::buttonNormal));
  button->setColour(
      juce::TextButton::buttonDown, findColour(juce::Button::buttonDown));
  button->setColour(
      juce::TextButton::textColourOffId,
      findColour(juce::TextButton::textColourOffId));
  button->setColour(
      juce::TextButton::textColourOnId,
      findColour(juce::TextButton::textColourOnId));
  widthOfCrumbs += button->getWidth();
  addAndMakeVisible(button);
  components.push_back(std::unique_ptr<juce::Component>(button));
}

void BreadCrumb::buttonClicked(juce::Button *button) {
  auto itr = std::find_if(components.begin(), components.end(), [=](auto &b) {
    return b.get() == button;
  });
  if (itr != components.end()) {
    size_t dist = std::distance(components.begin(), itr);
    if (dist % 2 == 0) { // have a crumb
      juce::StringArray array;
      for (size_t i = 0; i <= dist / 2; ++i)
        array.add(crumbStrings[i]);
      notifyListeners(array);
    }
  }
}

void BreadCrumb::paint(juce::Graphics &g) {
  g.setColour(findColour(juce::TableHeaderComponent::backgroundColourId));
  g.fillAll();
};
void BreadCrumb::resized() {
  for (auto &child : components)
    child->setBounds(child->getBounds().withHeight(getHeight()));
}

void BreadCrumb::notifyListeners(const juce::StringArray &array) {
  for (auto listener : listeners)
    listener->breadCrumbClicked(array);
}

void BreadCrumb::addListener(Listener *listener) {
  listeners.push_back(listener);
}
void BreadCrumb::removeListener(Listener *listener) {
  auto itr = std::find(listeners.begin(), listeners.end(), listener);
  if (itr != listeners.end())
    listeners.erase(itr);
}
