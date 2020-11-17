/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

/*
 TO DO:
 click anywhere on the window and play a note
 if you click and drag it will change the pitch of a note.
 Should we play a sound?
 */

#pragma once

#include <JuceHeader.h>
//==============================================================================
#include<array>
struct BufferAnalyzer : Thread, Timer, Component
{
    BufferAnalyzer();
    ~BufferAnalyzer();
    
    void prepare(double sampleRate, int samplesPerBlock);
    void cloneBuffer ( const dsp::AudioBlock<float>& other);
    void run() override;
    void timerCallback() override;
    void paint(Graphics& g) override;
    
private:
    std::array<AudioBuffer<float>, 2> buffers;
    Atomic<bool> firstBuffer {true};
    
    std::array<Atomic<size_t>, 2> samplesCopied;
    
    //===========================
    enum
    {
        fftOrder  = 11,
        fftSize   = 1 << fftOrder,
        numPoints = 512
    };
    
    float fifoBuffer [fftSize];
    float fftData [2 * fftSize];
    int fifoIndex = 0;
    
    void pushNextSampleIntoFifo (float sample);
    
    Atomic<bool> nextFFTBlockReady = false;
    float curveData [numPoints];
    
    dsp::FFT forwardFFT{fftOrder};
    
    dsp::WindowingFunction<float> window {fftSize, juce::dsp::WindowingFunction<float>::hann};
    
    void drawNextFrameOfSpectrum();
    Path fftCurve;
};

//==============================================================================
/**
*/
class Pfmproject0AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Pfmproject0AudioProcessor();
    ~Pfmproject0AudioProcessor();

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
    AudioParameterBool* shouldPlaySound = nullptr; // = false;
    
    AudioParameterFloat* bgColor = nullptr;
    
    static void UpdateAutomatableParameter(RangedAudioParameter*, float value);
    
        BufferAnalyzer leftBufferAnalyzer, rightBufferAnalyzer;
    
private:
    AudioProcessorValueTreeState apvts;
    Random r;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmproject0AudioProcessor)
};
