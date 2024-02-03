#include <juce_audio_basics/juce_audio_basics.h>
template <typename SampleType> class IBufferMixer {
public:
  ~IBufferMixer() = default;
  virtual void
  mix(juce::AudioBuffer<SampleType> output,
      juce::AudioBuffer<SampleType> a,
      juce::AudioBuffer<SampleType> b) = 0;
};
