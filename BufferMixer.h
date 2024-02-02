#include <juce_audio_basics/juce_audio_basics.h>
template <typename SampleType> class BufferMixer {
public:
  ~BufferMixer() = default;
  virtual void
  mix(juce::AudioBuffer<SampleType> output,
      juce::AudioBuffer<SampleType> a,
      juce::AudioBuffer<SampleType> b) = 0;
};

template <typename SampleType>
class LinearBufferMixer : private BufferMixer<SampleType> {
private:
  float wetLevel;

public:
  void setWetLevel(float level) { wetLevel = level; }
  float getWetLevel() { return wetLevel; }
  void
  mix(juce::AudioBuffer<SampleType> output,
      juce::AudioBuffer<SampleType> a,
      juce::AudioBuffer<SampleType> b) override {
    for (auto channel = 0; channel < output.getNumChannels(); ++channel) {
      auto *writeBuffer = output.getWritePointer(channel);
      auto *readBufferA = a.getReadPointer(channel);
      auto *readBufferB = b.getReadPointer(channel);
      for (auto sample = 0; sample < output.getNumSamples(); ++sample) {
        writeBuffer[sample] = (1 - wetLevel) * readBufferA[sample] +
                              wetLevel * readBufferB[sample];
      }
    }
  }
};
