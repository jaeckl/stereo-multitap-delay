#include "IPreset.h"
#include <memory>
#include <vector>
class FilePreset : public IPreset {
public:
  FilePreset(juce::String name, std::vector<juce::Point<float>> points);
  static std::unique_ptr<FilePreset> fromMemory(const char *name, size_t size);
  static std::unique_ptr<FilePreset> fromFile(const juce::File &file);
  static std::unique_ptr<FilePreset> fromString(juce::String xmlString);
  void initialize(double sampleRate, double bpm) override;

  const std::vector<juce::Point<float>> &getPresetPoints() const override;
  juce::String getName() override;

private:
  juce::String presetName;
  std::vector<juce::Point<float>> presetPoints;
};
