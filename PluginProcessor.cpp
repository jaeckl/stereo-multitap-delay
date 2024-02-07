#include "PluginProcessor.h"
#include "Audio/StereoDelayLineConfig.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              )
    , delayLineConfig(new StereoDelayLineConfig)
    , isLowpassFilter(false)
    , isHighpassFilter(false)
    , filterValue(0.0f)
    , filterValueChanged(false) {
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() { return 0; }

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(
    int index,
    const juce::String &newName) {
  juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(
    double sampleRate,
    int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  juce::ignoreUnused(sampleRate, samplesPerBlock);
  juce::dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = samplesPerBlock;
  spec.numChannels = getTotalNumOutputChannels();
  inputFilter.reset(new juce::dsp::StateVariableTPTFilter<float>());
  inputFilter->prepare(spec);
  delayLineConfig->init(sampleRate);
}

void AudioPluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(
    juce::AudioBuffer<float> &buffer,
    juce::MidiBuffer &midiMessages) {
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  if (!isBypassing) {
    bpm = getPlayHead()->getPosition()->getBpm().orFallback(120);
    if (!isBufferInitialized) {
      delayLineConfig->init(samplesPerBar());
      isBufferInitialized = true;
    }

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
      buffer.clear(i, 0, buffer.getNumSamples());

    juce::AudioBuffer<float> wetBuffer(
        buffer.getNumChannels(), buffer.getNumSamples());
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
      wetBuffer.copyFrom(
          channel, 0, buffer, channel, 0, buffer.getNumSamples());

    if (filterValueChanged) {
      inputFilter->setCutoffFrequency(std::pow(10, filterValue));
      if (isLowpassFilter && inputFilter->getType() !=
                                 juce::dsp::StateVariableTPTFilterType::lowpass)
        inputFilter->setType(juce::dsp::StateVariableTPTFilterType::lowpass);
      if (isHighpassFilter &&
          inputFilter->getType() !=
              juce::dsp::StateVariableTPTFilterType::highpass)
        inputFilter->setType(juce::dsp::StateVariableTPTFilterType::highpass);
      filterValueChanged = false;
    }
    if (isHighpassFilter || isLowpassFilter) {
      juce::dsp::AudioBlock<float> block(wetBuffer);
      juce::dsp::AudioBlock<float> singleChannelBlock(wetBuffer);
      juce::dsp::ProcessContextReplacing<float> context(singleChannelBlock);
      inputFilter->process(context);
    }

    delayLineConfig->processBlock(wetBuffer);
    bufferMixer.setWetLevel(delayMix);
    bufferMixer.mix(buffer, buffer, wetBuffer);

    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
      auto *readBuffer = buffer.getReadPointer(channel);
      auto writeBuffer = buffer.getWritePointer(channel);

      for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
        writeBuffer[sample] =
            juce::Decibels::decibelsToGain(outputVolume) * readBuffer[sample];
      }
    }
  }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor() {
  return new ProcessorEditor(*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void AudioPluginAudioProcessor::setStateInformation(
    const void *data,
    int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::ignoreUnused(data, sizeInBytes);
}

int AudioPluginAudioProcessor::samplesPerBar() {
  return 4 * getSampleRate() * 60 / bpm;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new AudioPluginAudioProcessor();
}
