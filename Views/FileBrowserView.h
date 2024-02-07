#pragma once
#include "Components/CustomFileListComponent.h"
#include "Components/UIElementPlacer.h"
#include "LookAndFeel/OnScreenFlatLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>
class FileBrowserView
    : public juce::Component
    , private juce::Button::Listener
    , private juce::FileBrowserListener {
public:
  class Listener {
  public:
    ~Listener() = default;
    virtual void fileAccepted(const juce::File &file) = 0;
  };
  FileBrowserView(const juce::String &name);
  void paint(juce::Graphics &g) override;
  void resized() override;
  void fileDoubleClicked(const juce::File &file) override;
  void selectionChanged() override{};
  void fileClicked(const juce::File &file, const juce::MouseEvent &e) override {
  }
  void buttonClicked(juce::Button *) override;
  void browserRootChanged(const juce::File &newRoot) override {}

  void addListener(Listener *listener);
  void removeListener(Listener *listener);

private:
  OnScreenFlatLookAndFeel flatLookAndFeel;
  juce::TimeSliceThread thread;
  juce::WildcardFileFilter wildCardFilter;
  juce::DirectoryContentsList contentList;
  juce::ImageButton backButton;
  juce::File filePath;
  juce::Label folderPathLabel;
  juce::Label notFoundLabel;
  CustomFileListComponent fileListComponent;
  XMLElementLayouter layouter;

  bool isNoPresetPresent();
  void updateFileBrowserDirectory(const juce::File &file);
  void configureBackButton();
  void configureFolderPathLabel();
  void configureNotFoundLabel();
  void configureFileList();
  void addControlsToView();
  void notifyListeners(const juce::File &file);
  std::vector<Listener *> listeners;
};
