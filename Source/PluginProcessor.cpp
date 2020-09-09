/*
  ==============================================================================

    Julian's Biquad Filter
 
    A variable bandpass filter plugin built from delay lines. Easily alterable
    to other filter states, and based on the Bandpass filter described in DAFX
    (Zolzer, p.43).
 
 
             ┌---- A(z) ----┐
    x[n] ----┤            (+/-) --- (*0.5) ---> y[n]
             └--------------┘
 
    Where A(z) is a second order allpass filter. The filter coefficients are
    stored in Array<float>s a and b, both processor properties. These can be
    changed and extended to permit other filter types of higher order, as
    long as a and b are the same size.
 
    This is an experiment in filter implementation more than in filter design,
    and is for educational purposes. For a long time real-time filter implement-
    -ation was a mystery and building this plugin made the process concrete.
 
    Included is the object FilterDelayLine that is used to handle reading and
    writing to a circular buffer. Two lines are used: (1) to handle feed for-
    -ward signal and (2) to handle feedback signal.
 
    Julian Armand Vanasse
    McGill University, Music Technology Area
    Fall 2020.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define PI 3.141592653589793

//==============================================================================
FilterAudioProcessor::FilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
    
    // init delay lines
    ffdelay = FilterDelayLine(3);
    fbdelay = FilterDelayLine(3);
    
    // filter constants
    fc = 1000.0f;
    fb = 1000.0f;
    
    p = tan(PI*(fb/fs));
    c = (p-1) / (p+1);
    d = -cos(2.0f*PI*(fc/fs));
    
    // filter coefficients
    Array<float> a = Array<float>();
    Array<float> b = Array<float>();
    
    a.add(1.0f);
    a.add(d*(1-c));
    a.add(-c);
    
    b.add(-c);
    b.add(d*(1-c));
    b.add(1.0f);
    
}

FilterAudioProcessor::~FilterAudioProcessor()
{
}

//==============================================================================
const String FilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterAudioProcessor::setCurrentProgram (int index)
{
}

const String FilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    fs = static_cast<float>(sampleRate);
    
}

void FilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    
    update_coefficients();
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Overwrite unused channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Filter used channel
    for (int n = 0; n < buffer.getNumSamples(); n++)
    {
        float current_sample = buffer.getSample(0, n);
        ffdelay.write(current_sample);
        
        float ap_out = 0.0f;
        
        ap_out = difference_equation(current_sample);
        float bp_out = 0.5f * (current_sample - ap_out);
        
        
        buffer.setSample(0, n, bp_out * gain);
        
        fbdelay.write(ap_out);
        
        ffdelay.advance();
        fbdelay.advance();
    }
}

float FilterAudioProcessor::difference_equation(float current_sample)
{
    float out = 0.0f;
    
    for (int i = 0; i < ffdelay.getSize(); i++)
    {
        if (i > 0)
        {
            out = out + (b[i] * ffdelay.read(i)) - (a[i] * fbdelay.read(i));
        }
        else
        {
            out = out + (b[i] * ffdelay.read(i));
        }
    }
    
    return out;
}

void FilterAudioProcessor::setCutoff(float cutoff)
{
    fc = cutoff;
}

void FilterAudioProcessor::setBandwidth(float bw)
{
    fb = bw;
}

void FilterAudioProcessor::setGain(float vol)
{
    gain = vol;
}

void FilterAudioProcessor::update_coefficients()
{
    p = tan(PI*(fb/fs));
    c = (p-1) / (p+1);
    d = -cos(2*PI*(fc/fs));
    
    a.set(0, 1.0f);
    a.set(1, d*(1-c));
    a.set(2, -c);
    
    b.set(0, -c);
    b.set(1, d*(1-c));
    b.set(2, 1.0f);
    
}

//==============================================================================
bool FilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
    return new FilterAudioProcessorEditor (*this);
}

//==============================================================================
void FilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterAudioProcessor();
}
