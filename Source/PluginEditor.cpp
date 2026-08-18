#include "PluginEditor.h"

AudioProgrammerPluginAudioProcessorEditor::AudioProgrammerPluginAudioProcessorEditor(
    AudioProgrammerPluginAudioProcessor& processorToUse)
    : AudioProcessorEditor(&processorToUse),
      gainAttachment(processorToUse.getParameters(), "gain", gainSlider)
{
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 24);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(gainSlider);

    gainLabel.setText("OUTPUT GAIN", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainSlider, false);
    addAndMakeVisible(gainLabel);

    setResizable(true, true);
    setResizeLimits(280, 220, 800, 600);
    setSize(420, 320);
}

void AudioProgrammerPluginAudioProcessorEditor::paint(juce::Graphics& graphics)
{
    graphics.fillAll(juce::Colour::fromRGB(24, 27, 32));

    graphics.setColour(juce::Colour::fromRGB(235, 238, 245));
    graphics.setFont(juce::FontOptions(24.0f).withStyle("Bold"));
    graphics.drawFittedText("Audio Programmer Plugin",
                            getLocalBounds().removeFromTop(64),
                            juce::Justification::centred,
                            1);

    graphics.setColour(juce::Colour::fromRGB(139, 147, 166));
    graphics.setFont(juce::FontOptions(14.0f));
    graphics.drawFittedText("CMake + JUCE effects template",
                            getLocalBounds().removeFromBottom(40),
                            juce::Justification::centred,
                            1);
}

void AudioProgrammerPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(32);
    bounds.removeFromTop(64);
    bounds.removeFromBottom(32);
    gainSlider.setBounds(bounds.withSizeKeepingCentre(160, 160));
}
