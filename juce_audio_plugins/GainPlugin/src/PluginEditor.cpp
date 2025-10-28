#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainPluginAudioProcessorEditor::GainPluginAudioProcessorEditor(GainPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set editor size
    setSize(400, 300);

    // Configure gain label
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    // Configure gain slider
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setRange(0.0, 2.0, 0.01);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    gainSlider.setPopupDisplayEnabled(true, false, this);
    gainSlider.setTextValueSuffix(" Gain");
    gainSlider.setValue(audioProcessor.getGainParameter()->get());
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);

    // Configure value label
    valueLabel.setText(juce::String(gainSlider.getValue(), 2), juce::dontSendNotification);
    valueLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    valueLabel.setJustificationType(juce::Justification::centred);
    valueLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(valueLabel);
}

GainPluginAudioProcessorEditor::~GainPluginAudioProcessorEditor()
{
}

//==============================================================================
void GainPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(juce::Colours::darkgrey);

    // Draw title
    g.setColour(juce::Colours::white);
    g.setFont(28.0f);
    g.drawFittedText("Simple Gain Plugin", getLocalBounds().removeFromTop(60),
                     juce::Justification::centred, 1);

    // Draw border
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(getLocalBounds(), 2);
}

void GainPluginAudioProcessorEditor::resized()
{
    // Layout components
    auto bounds = getLocalBounds();

    // Title area
    bounds.removeFromTop(60);

    // Label
    gainLabel.setBounds(bounds.removeFromTop(30).reduced(10, 0));

    // Slider
    gainSlider.setBounds(bounds.removeFromTop(150).reduced(50, 20));

    // Value label
    valueLabel.setBounds(bounds.removeFromTop(40).reduced(10, 0));
}

void GainPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        // Update the parameter
        audioProcessor.getGainParameter()->setValueNotifyingHost(
            static_cast<float>(gainSlider.getValue()));

        // Update value label
        valueLabel.setText(juce::String(gainSlider.getValue(), 2),
                          juce::dontSendNotification);
    }
}
