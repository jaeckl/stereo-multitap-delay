#include "FileBrowserView.h"
#include "Constants.h"
#include <BinaryData.h>
static juce::String getRootFolderPath() {
#if JUCE_WINDOWS
  // On Windows, the root of the current drive can be referred to as "C:\", for
  // example. Assuming you want to get the root of the drive where the
  // application is running:
  return File::getSpecialLocation(
             File::SpecialLocationType::currentExecutableFile)
      .getRoot();
#elif JUCE_MAC
  // On macOS, the root folder is simply "/"
  return "/";
#elif JUCE_LINUX
  // On Linux, the root folder is also "/"
  return "/";
#else
#error Unsupported platform
#endif
}
FileBrowserView::FileBrowserView(const juce::String &name)
    : whiteButtonStyle(Constants::WHITE_COLOUR, true)
    , thread("")
    , wildCardFilter(
          Constants::TEXT_PRESET_FILE_WILDCARD,
          Constants::TEXT_PRESET_FOLDER_WILDCARD,
          "Custom File Filter")
    , contentList(&wildCardFilter, thread)
    , backButton(Constants::COMPONENT_BUTTON_BACK)

    , filePath(getRootFolderPath())

    , folderPathLabel(
          Constants::COMPONENT_TEXT_LABEL_PATH,
          filePath.getFullPathName())
    , notFoundLabel(
          Constants::COMPONENT_LABEL_NO_PRESET,
          Constants::TEXT_PRESETS_NOT_FOUND)
    , fileListComponent(Constants::COMPONENT_FILELIST, contentList)
    , layouter(this, BinaryData::filebrowser_layout_xml) {

  setName(name);
  thread.startThread();
  contentList.setDirectory(filePath, true, true);

  configureBackButton();
  configureFileList();
  configureFolderPathLabel();
  configureNotFoundLabel();
  addControlsToView();

  layouter.updateComponentBounds();
}
void FileBrowserView::paint(juce::Graphics &g) {
  g.setColour(juce::Colours::white);
  g.drawRect({getLocalBounds().reduced(20, 20)}, 2);
  g.fillRect(getLocalBounds().reduced(20, 20).withHeight(28));
}
void FileBrowserView::resized() { layouter.updateComponentBounds(); }
void FileBrowserView::buttonClicked(juce::Button *button) {
  if (button == &backButton) {
    if (!filePath.isRoot()) {
      updateFileBrowserDirectory(filePath.getParentDirectory());
      if (isNoPresetPresent())
        notFoundLabel.setVisible(true);
      else
        notFoundLabel.setVisible(false);
    }
  }
}
void FileBrowserView::fileDoubleClicked(const juce::File &file) {
  if (file.isDirectory()) {
    updateFileBrowserDirectory(file);
    if (isNoPresetPresent())
      notFoundLabel.setVisible(true);
    else
      notFoundLabel.setVisible(false);
  } else {
    notifyListeners(file);
  }
}

bool FileBrowserView::isNoPresetPresent() {
  juce::RangedDirectoryIterator dirIterator(
      filePath, false, "*", juce::File::findDirectories);

  bool hasSubdirectories = false;

  for (auto dir : dirIterator) {
    if (dir.getFile().isDirectory()) {
      hasSubdirectories = true;
      break;
    }
  }
  if (contentList.getNumFiles() == 0 && !hasSubdirectories) {
    return true;
  }
  return false;
}

void FileBrowserView::updateFileBrowserDirectory(const juce::File &file) {
  filePath = file;
  folderPathLabel.setText(
      file.getFullPathName(), juce::NotificationType::dontSendNotification);
  contentList.setDirectory(file, true, true);
  fileListComponent.selectRow(0);
}

void FileBrowserView::configureBackButton() {
  auto instream = juce::MemoryInputStream(
      BinaryData::icon_arrow_left_png,
      BinaryData::icon_arrow_left_pngSize,
      false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(instream);
  auto image = format->decodeImage(instream);
  backButton.setImages(
      true,
      true,
      true,
      image,
      1.0,
      juce::Colours::transparentWhite,
      image,
      1.0,
      juce::Colours::transparentWhite.withAlpha(0.2f),
      image,
      1.0,
      juce::Colours::transparentWhite.withAlpha(0.4f));
  backButton.addListener(this);
}

void FileBrowserView::addControlsToView() {
  addAndMakeVisible(backButton);
  addAndMakeVisible(folderPathLabel);
  addAndMakeVisible(notFoundLabel);
  notFoundLabel.setVisible(false);
  addAndMakeVisible(fileListComponent);
}

void FileBrowserView::configureNotFoundLabel() {
  notFoundLabel.setJustificationType(juce::Justification::centred);
}
void FileBrowserView::configureFileList() {
  fileListComponent.setLookAndFeel(&whiteButtonStyle);
  fileListComponent.addListener(this);
}

void FileBrowserView::configureFolderPathLabel() {
  folderPathLabel.setColour(juce::Label::textColourId, juce::Colours::black);
}

void FileBrowserView::addListener(Listener *listener) {
  listeners.push_back(listener);
}
void FileBrowserView::removeListener(Listener *listener) {
  auto itr = std::find(listeners.begin(), listeners.end(), listener);
  if (itr != listeners.end())
    listeners.erase(itr);
}

void FileBrowserView::notifyListeners(const juce::File &file) {
  for (auto &listener : listeners) {
    listener->fileAccepted(file);
  }
}
