#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 * Simple Gain Plugin Editor with a slider control
 */
class GainPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       private juce::Slider::Listener
{
public:
    GainPluginAudioProcessorEditor(GainPluginAudioProcessor&);
    ~GainPluginAudioProcessorEditor() override;

    //==========================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainPluginAudioProcessor& audioProcessor;

    // UI Components
    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::Label valueLabel;

    // Slider listener
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainPluginAudioProcessorEditor)
};
