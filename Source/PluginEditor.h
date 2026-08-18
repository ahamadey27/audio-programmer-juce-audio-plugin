#pragma once

#include "PluginProcessor.h"
#include <juce_gui_extra/juce_gui_extra.h>

class AudioProgrammerPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioProgrammerPluginAudioProcessorEditor(AudioProgrammerPluginAudioProcessor&);
    ~AudioProgrammerPluginAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioProgrammerPluginAudioProcessorEditor)
};
