/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterDelayLine.h"

//==============================================================================
/**
*/
class FilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    FilterAudioProcessor();
    ~FilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void setBandwidth(float bw);
    void setCutoff(float cutoff);
    void setGain(float vol);

private:
    //==============================================================================
    
    FilterDelayLine ffdelay;
    FilterDelayLine fbdelay;
    
    Array<float> a;
    Array<float> b;
    
    float p;
    float c;
    float d;
    
    float fc;
    float fb;
    float fs {44100.0f};       // sample rate
    
    float difference_equation(float current_sample);
    void update_coefficients();
    
    float gain {1.0f};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessor)
};
