#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProgrammerPluginAudioProcessor::AudioProgrammerPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "Parameters", createParameterLayout()),
      gainDb(parameters.getRawParameterValue("gain"))
{
    jassert(gainDb != nullptr);
}

const juce::String AudioProgrammerPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioProgrammerPluginAudioProcessor::acceptsMidi() const { return false; }
bool AudioProgrammerPluginAudioProcessor::producesMidi() const { return false; }
bool AudioProgrammerPluginAudioProcessor::isMidiEffect() const { return false; }
double AudioProgrammerPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioProgrammerPluginAudioProcessor::getNumPrograms() { return 1; }
int AudioProgrammerPluginAudioProcessor::getCurrentProgram() { return 0; }
void AudioProgrammerPluginAudioProcessor::setCurrentProgram(int) {}
const juce::String AudioProgrammerPluginAudioProcessor::getProgramName(int) { return {}; }
void AudioProgrammerPluginAudioProcessor::changeProgramName(int, const juce::String&) {}

void AudioProgrammerPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    const juce::dsp::ProcessSpec spec {
        sampleRate,
        static_cast<juce::uint32>(samplesPerBlock),
        static_cast<juce::uint32>(getTotalNumOutputChannels())
    };

    outputGain.prepare(spec);
    outputGain.setRampDurationSeconds(0.02);
    outputGain.setGainDecibels(gainDb->load());
}

void AudioProgrammerPluginAudioProcessor::releaseResources() {}

bool AudioProgrammerPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto outputLayout = layouts.getMainOutputChannelSet();

    if (outputLayout != juce::AudioChannelSet::mono()
        && outputLayout != juce::AudioChannelSet::stereo())
        return false;

    return outputLayout == layouts.getMainInputChannelSet();
}

void AudioProgrammerPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                         juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    for (auto channel = getTotalNumInputChannels();
         channel < getTotalNumOutputChannels();
         ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());

    outputGain.setGainDecibels(gainDb->load());

    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);
    outputGain.process(context);
}

bool AudioProgrammerPluginAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* AudioProgrammerPluginAudioProcessor::createEditor()
{
    return new AudioProgrammerPluginAudioProcessorEditor(*this);
}

void AudioProgrammerPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destinationData)
{
    if (auto stateXml = parameters.copyState().createXml())
        copyXmlToBinary(*stateXml, destinationData);
}

void AudioProgrammerPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto stateXml = getXmlFromBinary(data, sizeInBytes))
        if (stateXml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*stateXml));
}

juce::AudioProcessorValueTreeState::ParameterLayout
AudioProgrammerPluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "gain", 1 },
        "Gain",
        juce::NormalisableRange<float> { -60.0f, 12.0f, 0.1f },
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProgrammerPluginAudioProcessor();
}

