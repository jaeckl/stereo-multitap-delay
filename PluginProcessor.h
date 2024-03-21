#pragma once

#include "Audio/IDelayLineConfig.h"
#include "Audio/Mixer/LinearBufferMixer.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <memory>
#include <vector>
//==============================================================================
class PluginProcessor final : public juce::AudioProcessor {
public:
  //==============================================================================
  PluginProcessor();
  ~PluginProcessor() override;

  float delayMix;
  float outputVolume;
  bool isLowpassFilter;
  bool isHighpassFilter;
  bool filterValueChanged;
  float filterValue;

  LinearBufferMixer<float> bufferMixer;
  std::shared_ptr<IDelayLineConfig> delayLineConfig;
  bool isBufferInitialized = false;
  bool isBypassing = false;

  int maxDelayInSamples() { return samplesPerBar(); }
  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
  using AudioProcessor::processBlock;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  int samplesPerBar();

private:
  double bpm = 120;
  std::unique_ptr<juce::dsp::StateVariableTPTFilter<float>> inputFilter;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
