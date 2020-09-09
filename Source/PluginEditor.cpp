/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 150);
    
    cutoffSlider.setSize(380, 20);
    bandwidthSlider.setSize(380, 20);
    gainSlider.setSize(380, 20);
    
    cutoffSlider.setRange(40.0f, 15000.0f);
    bandwidthSlider.setRange(20.0f, 4000.0f);
    gainSlider.setRange(0.0f, 10.0f);
    
    cutoffSlider.setValue(1000.0f);
    bandwidthSlider.setValue(1000.0f);
    gainSlider.setValue(1.0f);
    
    addAndMakeVisible(cutoffSlider);
    addAndMakeVisible(bandwidthSlider);
    addAndMakeVisible(gainSlider);
    
    cutoffSlider.addListener(this);
    bandwidthSlider.addListener(this);
    gainSlider.addListener(this);
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor()
{
}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Julian's Biquad Super Fun Sunny All The Time Filter.", getLocalBounds(), Justification::centredBottom, 1);
    
    
}

void FilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    cutoffSlider.setBounds(20, 20, 400, 20);
    bandwidthSlider.setBounds(20, 50, 400, 20);
    gainSlider.setBounds(20, 80, 400, 20);
}

void FilterAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &cutoffSlider)
    {
        float value = static_cast<float>( slider->getValue() );
        processor.setCutoff(value);
    }
    
    if (slider == &bandwidthSlider)
    {
        float value = static_cast<float>(slider->getValue());
        processor.setBandwidth(value);
    }
    
    if (slider == &gainSlider)
    {
        float value = static_cast<float>(slider->getValue());
        processor.setGain(value);
    }

}
