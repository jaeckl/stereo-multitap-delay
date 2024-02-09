#include "SingleDelayEditorConfig.h"
#include "Audio/MonoDelayLineConfig.h"
#include "Audio/StereoDelayLineConfig.h"
void SingleDelayEditorConfig::initialize(
    std::shared_ptr<IDelayLineConfig> config,
    AudioPluginAudioProcessor *processor) {
  processorRef = processor;
  if (auto conf = std::dynamic_pointer_cast<MonoDelayLineConfig>(config)) {
    delayLineConfig = conf;
    isStereoDelay = false;
    delayLineConfig->clear();
    for (auto &point : points) {
      int delayInSamples =
          static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
      delayLineConfig->addDelayLineReader(
          0, delayInSamples, 1.0f - point.getY());
    }

    return;
  }
  if (auto conf = std::dynamic_pointer_cast<StereoDelayLineConfig>(config)) {
    delayLineConfig = conf;
    isStereoDelay = true;
    delayLineConfig->clear();
    for (auto &point : points) {
      int delayInSamples =
          static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
      delayLineConfig->addDelayLineReader(
          0, delayInSamples, 1.0f - point.getY());
      delayLineConfig->addDelayLineReader(
          1, delayInSamples, 1.0f - point.getY());
    }
    return;
  }
  throw std::exception();
}

void SingleDelayEditorConfig::addPoint(const juce::Point<float> &point) {
  points.push_back(point);
  int delayInSamples =
      static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
  if (isStereoDelay) {
    auto config =
        std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
    config->addDelayLineReader(0, delayInSamples, 1.0f - point.getY());
    config->addDelayLineReader(1, delayInSamples, 1.0f - point.getY());
  } else {
    auto config =
        std::dynamic_pointer_cast<MonoDelayLineConfig>(delayLineConfig);
    config->addDelayLineReader(0, delayInSamples, 1.0f - point.getY());
  }
}

void SingleDelayEditorConfig::removePoint(int pointIndex) {
  points.erase(points.begin() + pointIndex);
  if (isStereoDelay) {
    auto config =
        std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
    config->removeDelayLineReader(0, pointIndex);
    config->removeDelayLineReader(1, pointIndex);
  } else {
    auto config =
        std::dynamic_pointer_cast<MonoDelayLineConfig>(delayLineConfig);
    config->removeDelayLineReader(0, pointIndex);
  }
}
juce::Point<float> SingleDelayEditorConfig::getPoint(int pointIndex) {
  return points[static_cast<size_t>(pointIndex)];
}
void SingleDelayEditorConfig::clearPoints() {
  points.clear();
  delayLineConfig->clear();
}

int SingleDelayEditorConfig::getNumPoints() { return points.size(); }
void SingleDelayEditorConfig::insertPoint(
    int pointIndex,
    const juce::Point<float> &point) {
  if (pointIndex == points.size()) {
    addPoint(point);
    return;
  }
  points.insert(points.begin() + pointIndex, point);
  int delayInSamples =
      static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
  if (isStereoDelay) {
    auto config =
        std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
    config->insertDelayLineValue(
        0, pointIndex, delayInSamples, 1.0f - point.getY());
    config->insertDelayLineValue(
        1, pointIndex, delayInSamples, 1.0f - point.getY());
  } else {
    auto config =
        std::dynamic_pointer_cast<MonoDelayLineConfig>(delayLineConfig);
    config->insertDelayLineValue(
        0, pointIndex, delayInSamples, 1.0f - point.getY());
  }
}
void SingleDelayEditorConfig::resetPoint(
    int pointIndex,
    const juce::Point<float> &point) {
  points[static_cast<size_t>(pointIndex)] = point;
  int delayInSamples =
      static_cast<int>(point.getX() * processorRef->maxDelayInSamples());
  if (isStereoDelay) {
    auto config =
        std::dynamic_pointer_cast<StereoDelayLineConfig>(delayLineConfig);
    config->setDelayLineValue(
        0, pointIndex, delayInSamples, 1.0f - point.getY());
    config->setDelayLineValue(
        1, pointIndex, delayInSamples, 1.0f - point.getY());
  } else {
    auto config =
        std::dynamic_pointer_cast<MonoDelayLineConfig>(delayLineConfig);
    config->setDelayLineValue(
        0, pointIndex, delayInSamples, 1.0f - point.getY());
  }
}
std::vector<juce::Point<float>> SingleDelayEditorConfig::getPoints() {
  return points;
}

void SingleDelayEditorConfig::addListener(Listener *listener) {
  listeners.push_back(listener);
}

void SingleDelayEditorConfig::removeListener(Listener *listener) {
  auto it = std::find(listeners.begin(), listeners.end(), listener);
  if (it != listeners.end())
    listeners.erase(it);
}
