# JUCE Audio Processing Knowledge Base

## Overview

This knowledge base documents audio processing concepts, architecture, and implementation patterns in JUCE (Jules' Utility Class Extensions) - a comprehensive C++ framework for audio application development.

**Target Audience**: Firmware engineers, audio DSP developers, plugin developers
**Framework Version**: JUCE 7.0.12+
**Language**: C++17
**Focus Areas**: Real-time audio processing, DSP algorithms, audio I/O management

---

## Table of Contents

1. [Audio Processing Architecture](#audio-processing-architecture)
2. [Core Audio Classes](#core-audio-classes)
3. [Audio Callback Mechanism](#audio-callback-mechanism)
4. [Audio Processing Patterns](#audio-processing-patterns)
5. [DSP Module Reference](#dsp-module-reference)
6. [Buffer Management](#buffer-management)
7. [Sample Rate and Block Size](#sample-rate-and-block-size)
8. [Thread Safety and Real-Time Constraints](#thread-safety-and-real-time-constraints)
9. [Audio Plugin Architecture](#audio-plugin-architecture)
10. [Performance Optimization](#performance-optimization)

---

## Audio Processing Architecture

### High-Level Overview

```
┌─────────────────────────────────────────────────────────┐
│                   JUCE Application                       │
│  ┌────────────────────────────────────────────────┐    │
│  │          Audio Plugin / Standalone App          │    │
│  │                                                  │    │
│  │  ┌──────────────────────────────────────┐     │    │
│  │  │   AudioProcessor                      │     │    │
│  │  │   - processBlock()                    │     │    │
│  │  │   - prepareToPlay()                   │     │    │
│  │  │   - releaseResources()                │     │    │
│  │  └──────────────────────────────────────┘     │    │
│  │           ↕                                     │    │
│  │  ┌──────────────────────────────────────┐     │    │
│  │  │   AudioDeviceManager                  │     │    │
│  │  │   (Manages hardware I/O)              │     │    │
│  │  └──────────────────────────────────────┘     │    │
│  └────────────────────────────────────────────────┘    │
│                       ↕                                  │
│  ┌────────────────────────────────────────────────┐    │
│  │          Operating System Audio API             │    │
│  │     (ASIO, CoreAudio, ALSA, WASAPI, etc.)      │    │
│  └────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
                         ↕
              ┌─────────────────────┐
              │   Audio Hardware     │
              │   (ADC/DAC)          │
              └─────────────────────┘
```

### Key Architectural Principles

1. **Real-Time Processing**: Audio callbacks run on high-priority threads with strict timing constraints
2. **Block-Based Processing**: Audio is processed in fixed-size buffers (typically 32-512 samples)
3. **Multi-Channel Support**: Native handling of mono, stereo, and multi-channel audio formats
4. **Plugin Formats**: Unified API supports VST, VST3, AU, AAX, LV2
5. **Cross-Platform**: Single codebase runs on Windows, macOS, Linux, iOS, Android

---

## Core Audio Classes

### 1. AudioProcessor

**Purpose**: Base class for all audio processing components (plugins, effects, instruments)

**Key Responsibilities**:
- Implement audio processing algorithm in `processBlock()`
- Manage audio/MIDI I/O configuration
- Handle parameter automation
- Save/restore state
- Provide editor (GUI) if needed

**Essential Virtual Methods**:

```cpp
class MyAudioProcessor : public juce::AudioProcessor
{
public:
    // Called before playback starts - allocate resources
    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;

    // Main audio processing callback - REAL-TIME THREAD
    void processBlock(juce::AudioBuffer<float>& buffer,
                      juce::MidiBuffer& midiMessages) override;

    // Called when playback stops - release resources
    void releaseResources() override;

    // Query supported bus layouts (channel configurations)
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    // Get/set state for session recall
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
};
```

**Lifecycle**:
```
Constructor → prepareToPlay() → [processBlock() × N] → releaseResources() → Destructor
                     ↑                                          ↓
                     └──────────── (loop if settings change) ───┘
```

---

### 2. AudioBuffer

**Purpose**: Container for multi-channel audio sample data

**Template Type**: `AudioBuffer<float>` or `AudioBuffer<double>`

**Key Operations**:

```cpp
// Get buffer properties
int numChannels = buffer.getNumChannels();
int numSamples = buffer.getNumSamples();

// Access channel data
float* channelData = buffer.getWritePointer(channelIndex);
const float* readData = buffer.getReadPointer(channelIndex);

// Get single sample
float sample = buffer.getSample(channel, sampleIndex);
buffer.setSample(channel, sampleIndex, newValue);

// Buffer operations
buffer.clear();                              // Zero all samples
buffer.applyGain(0.5f);                     // Multiply all by 0.5
buffer.addFrom(destChannel, 0, srcBuffer,   // Mix channels
               srcChannel, 0, numSamples);
```

**Memory Layout**:
```
Channel 0: [s0, s1, s2, ..., sN-1] ← contiguous float array
Channel 1: [s0, s1, s2, ..., sN-1]
Channel 2: [s0, s1, s2, ..., sN-1]
...
```

---

### 3. AudioDeviceManager

**Purpose**: Manages audio hardware devices and I/O routing

**Usage Example**:

```cpp
juce::AudioDeviceManager deviceManager;

// Initialize with default device
juce::String error = deviceManager.initialiseWithDefaultDevices(2, 2); // 2 in, 2 out

if (error.isNotEmpty())
{
    // Handle error
}

// Add audio callback
deviceManager.addAudioCallback(&myAudioCallback);

// Get current device info
juce::AudioIODevice* device = deviceManager.getCurrentAudioDevice();
double sampleRate = device->getCurrentSampleRate();
int bufferSize = device->getCurrentBufferSizeSamples();
```

---

### 4. AudioProcessorGraph

**Purpose**: Routing and mixing multiple audio processors

**Use Cases**:
- Effect chains
- Modular synthesizers
- Mixer applications
- Complex signal routing

**Example Structure**:
```cpp
juce::AudioProcessorGraph graph;

// Add nodes
auto nodeA = graph.addNode(std::make_unique<ProcessorA>());
auto nodeB = graph.addNode(std::make_unique<ProcessorB>());
auto audioIn = graph.addNode(std::make_unique<AudioGraphIOProcessor>(
    AudioGraphIOProcessor::audioInputNode));
auto audioOut = graph.addNode(std::make_unique<AudioGraphIOProcessor>(
    AudioGraphIOProcessor::audioOutputNode));

// Connect: Input → ProcessorA → ProcessorB → Output
graph.addConnection({{audioIn->nodeID, 0}, {nodeA->nodeID, 0}});
graph.addConnection({{nodeA->nodeID, 0}, {nodeB->nodeID, 0}});
graph.addConnection({{nodeB->nodeID, 0}, {audioOut->nodeID, 0}});
```

---

## Audio Callback Mechanism

### The processBlock() Method

**Signature**:
```cpp
void processBlock(juce::AudioBuffer<float>& buffer,
                  juce::MidiBuffer& midiMessages) override;
```

**Critical Characteristics**:
1. **Real-Time Thread**: Runs on high-priority audio thread
2. **Non-Blocking**: Must complete within buffer duration (e.g., 10ms @ 512 samples, 48kHz)
3. **No Allocations**: Never use `new`, `malloc`, `std::vector::push_back()`, etc.
4. **No Locks**: Avoid mutexes, prefer lock-free structures
5. **No I/O**: No file operations, network calls, or GUI updates

**Execution Flow**:

```cpp
void MyProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals; // Prevent denormal slowdown

    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    // 1. Clear unused output channels
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    // 2. Process MIDI events if needed
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        // Handle MIDI
    }

    // 3. Main DSP processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Per-sample processing
            channelData[sample] = processSample(channelData[sample], channel);
        }
    }

    // 4. Apply output gain, metering, etc.
    buffer.applyGain(outputGain.load());
}
```

---

## Audio Processing Patterns

### Pattern 1: Simple Gain Plugin

```cpp
class GainProcessor : public juce::AudioProcessor
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        // Initialize state
        currentSampleRate = sampleRate;
    }

    void processBlock(juce::AudioBuffer<float>& buffer,
                      juce::MidiBuffer&) override
    {
        // Apply gain to all channels
        buffer.applyGain(gainParameter->get());
    }

    void releaseResources() override
    {
        // Cleanup
    }

private:
    juce::AudioParameterFloat* gainParameter;
    double currentSampleRate = 44100.0;
};
```

---

### Pattern 2: State-Based Filter

```cpp
class FilterProcessor : public juce::AudioProcessor
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        // Initialize filter coefficients
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = getTotalNumOutputChannels();

        filter.prepare(spec);
        filter.reset();
    }

    void processBlock(juce::AudioBuffer<float>& buffer,
                      juce::MidiBuffer&) override
    {
        // Update filter parameters (smoothed)
        updateFilterCoefficients();

        // Process with JUCE DSP
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        filter.process(context);
    }

private:
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> filter;

    void updateFilterCoefficients()
    {
        auto freq = frequencyParam->get();
        auto q = qParam->get();
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
            currentSampleRate, freq, q);
    }
};
```

---

### Pattern 3: Delay Effect with Feedback

```cpp
class DelayProcessor : public juce::AudioProcessor
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        // Allocate circular buffer (2 seconds max)
        int maxDelaySamples = static_cast<int>(sampleRate * 2.0);
        delayBuffer.setSize(2, maxDelaySamples);
        delayBuffer.clear();
        writePosition = 0;
    }

    void processBlock(juce::AudioBuffer<float>& buffer,
                      juce::MidiBuffer&) override
    {
        const int numSamples = buffer.getNumSamples();
        const int delayInSamples = static_cast<int>(
            delayTimeMs->get() * 0.001 * currentSampleRate);
        const float feedback = feedbackParam->get();
        const float mix = mixParam->get();

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            float* delayData = delayBuffer.getWritePointer(channel);
            int bufferSize = delayBuffer.getNumSamples();

            for (int i = 0; i < numSamples; ++i)
            {
                // Read from delay buffer
                int readPos = (writePosition - delayInSamples + bufferSize) % bufferSize;
                float delayedSample = delayData[readPos];

                // Write to delay buffer (input + feedback)
                delayData[writePosition] = channelData[i] + delayedSample * feedback;

                // Mix dry and wet
                channelData[i] = channelData[i] * (1.0f - mix) + delayedSample * mix;

                writePosition = (writePosition + 1) % bufferSize;
            }
        }
    }

private:
    juce::AudioBuffer<float> delayBuffer;
    int writePosition = 0;
    juce::AudioParameterFloat* delayTimeMs;
    juce::AudioParameterFloat* feedbackParam;
    juce::AudioParameterFloat* mixParam;
};
```

---

## DSP Module Reference

JUCE provides a comprehensive DSP module (`juce_dsp`) with optimized algorithms:

### Available DSP Classes

| Class | Purpose | Typical Use |
|-------|---------|-------------|
| `dsp::Gain<T>` | Gain/attenuation | Volume control, mixing |
| `dsp::IIR::Filter<T>` | Infinite impulse response filter | EQ, tone shaping |
| `dsp::FIR::Filter<T>` | Finite impulse response filter | Linear-phase filtering |
| `dsp::StateVariableFilter` | Multi-mode filter | LP/HP/BP/Notch filtering |
| `dsp::Chorus<T>` | Chorus effect | Modulation effects |
| `dsp::Phaser<T>` | Phaser effect | Phase modulation |
| `dsp::Reverb` | Reverb algorithm | Spatial effects |
| `dsp::Convolution` | Convolution engine | Impulse responses, cab sims |
| `dsp::Compressor<T>` | Dynamics processor | Compression, limiting |
| `dsp::NoiseGate<T>` | Noise gate | Dynamics control |
| `dsp::Limiter<T>` | Peak limiter | Overload protection |
| `dsp::Oscillator<T>` | Waveform generator | Synthesis, LFOs |
| `dsp::FFT` | Fast Fourier Transform | Spectral analysis |
| `dsp::Oversampling<T>` | Oversampling engine | Anti-aliasing |

### DSP Processing Pattern

All `juce_dsp` processors follow this interface:

```cpp
// 1. Prepare (called once or when settings change)
juce::dsp::ProcessSpec spec;
spec.sampleRate = sampleRate;
spec.maximumBlockSize = samplesPerBlock;
spec.numChannels = numChannels;
dspModule.prepare(spec);

// 2. Reset state (optional)
dspModule.reset();

// 3. Process in audio callback
juce::dsp::AudioBlock<float> block(buffer);
juce::dsp::ProcessContextReplacing<float> context(block);
dspModule.process(context);
```

---

## Buffer Management

### Interleaved vs. Non-Interleaved

**JUCE uses non-interleaved format** (planar):

```cpp
// Non-interleaved (JUCE AudioBuffer)
Channel 0: [L0, L1, L2, L3, ...]
Channel 1: [R0, R1, R2, R3, ...]

// Interleaved (some hardware APIs)
Mixed: [L0, R0, L1, R1, L2, R2, ...]
```

### Zero-Copy Processing

Prefer in-place processing to avoid buffer copies:

```cpp
// Good: In-place processing
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            data[i] = processSample(data[i]); // Modify in place
    }
}

// Bad: Unnecessary copy
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
{
    juce::AudioBuffer<float> tempBuffer = buffer; // EXPENSIVE COPY!
    // ... process tempBuffer ...
    buffer = tempBuffer; // ANOTHER COPY!
}
```

---

## Sample Rate and Block Size

### Common Configurations

| Sample Rate | Use Case | Nyquist Frequency |
|-------------|----------|-------------------|
| 44.1 kHz | CD quality, consumer audio | 22.05 kHz |
| 48 kHz | Professional audio, video | 24 kHz |
| 88.2 kHz | High-resolution audio | 44.1 kHz |
| 96 kHz | Studio recording | 48 kHz |
| 192 kHz | Mastering, archival | 96 kHz |

| Block Size | Latency @ 48kHz | Use Case |
|------------|-----------------|----------|
| 32 samples | 0.67 ms | Ultra-low latency (live performance) |
| 64 samples | 1.33 ms | Low latency |
| 128 samples | 2.67 ms | Balanced |
| 256 samples | 5.33 ms | Standard |
| 512 samples | 10.67 ms | High CPU headroom |
| 1024 samples | 21.33 ms | Offline/non-real-time |

### Sample Rate Conversion

```cpp
void prepareToPlay(double sampleRate, int samplesPerBlock) override
{
    // Store for use in calculations
    currentSampleRate = sampleRate;

    // Example: Convert 1 kHz to samples per cycle
    samplesPerCycle = currentSampleRate / 1000.0;

    // Example: Convert 500ms to samples
    delaySamples = static_cast<int>(0.5 * currentSampleRate);
}
```

---

## Thread Safety and Real-Time Constraints

### Real-Time Safe Operations

**Allowed in processBlock()**:
- ✅ Stack allocations (local variables)
- ✅ Fixed-size array access
- ✅ Atomic operations (`std::atomic`, `juce::Atomic`)
- ✅ Lock-free queues
- ✅ Simple arithmetic
- ✅ Pre-allocated buffer access

**Forbidden in processBlock()**:
- ❌ `new`, `delete`, `malloc`, `free`
- ❌ `std::vector::push_back()`, `resize()`
- ❌ Mutexes (`std::mutex::lock()`)
- ❌ File I/O
- ❌ Network operations
- ❌ GUI updates
- ❌ System calls (e.g., `printf`)
- ❌ Exception throwing

### Lock-Free Communication

**Use `juce::AbstractFifo` for thread-safe queues**:

```cpp
class AudioProcessor
{
    juce::AbstractFifo fifo{1024}; // Ring buffer
    std::array<MidiMessage, 1024> buffer;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer& midi) override
    {
        // Audio thread: write MIDI to FIFO
        for (const auto metadata : midi)
        {
            int start1, size1, start2, size2;
            fifo.prepareToWrite(1, start1, size1, start2, size2);
            if (size1 > 0)
            {
                buffer[start1] = metadata.getMessage();
                fifo.finishedWrite(1);
            }
        }
    }

    void timerCallback() override
    {
        // GUI thread: read MIDI from FIFO
        int start1, size1, start2, size2;
        fifo.prepareToRead(1, start1, size1, start2, size2);
        if (size1 > 0)
        {
            auto message = buffer[start1];
            // Update GUI
            fifo.finishedRead(1);
        }
    }
};
```

### Parameter Smoothing

Prevent audio glitches when parameters change:

```cpp
class SmoothedParameter
{
    juce::SmoothedValue<float> smoothedGain;

    void prepareToPlay(double sampleRate, int)
    {
        smoothedGain.reset(sampleRate, 0.05); // 50ms ramp time
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
    {
        // Set target from parameter (can be called from GUI thread)
        smoothedGain.setTargetValue(gainParameter->get());

        // Apply smoothed value per-sample
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            float* data = buffer.getWritePointer(ch);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                float currentGain = smoothedGain.getNextValue();
                data[i] *= currentGain;
            }
        }
    }
};
```

---

## Audio Plugin Architecture

### AudioProcessor Structure

```
AudioProcessor (DSP Logic)
    ├── Parameters (AudioProcessorValueTreeState)
    ├── State Management (getStateInformation/setStateInformation)
    ├── Bus Configuration (input/output channels)
    └── Editor (GUI - optional)
            └── AudioProcessorEditor
```

### Parameter Management

**Best Practice: Use AudioProcessorValueTreeState**

```cpp
class MyProcessor : public juce::AudioProcessor
{
public:
    MyProcessor()
        : parameters(*this, nullptr, "PARAMETERS",
            {
                std::make_unique<juce::AudioParameterFloat>(
                    "gain", "Gain", 0.0f, 1.0f, 0.5f),
                std::make_unique<juce::AudioParameterFloat>(
                    "frequency", "Frequency",
                    juce::NormalisableRange<float>(20.0f, 20000.0f, 0.1f, 0.25f),
                    1000.0f)
            })
    {
        gainParam = parameters.getRawParameterValue("gain");
        freqParam = parameters.getRawParameterValue("frequency");
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        float gain = gainParam->load();
        float freq = freqParam->load();
        // Use parameters...
    }

private:
    juce::AudioProcessorValueTreeState parameters;
    std::atomic<float>* gainParam = nullptr;
    std::atomic<float>* freqParam = nullptr;
};
```

### Preset Management

```cpp
void getStateInformation(juce::MemoryBlock& destData) override
{
    // Serialize parameter state
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void setStateInformation(const void* data, int sizeInBytes) override
{
    // Deserialize parameter state
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}
```

---

## Performance Optimization

### 1. Use SIMD Instructions

```cpp
// Enable JUCE SIMD support
#define JUCE_USE_SIMD_EVERYWHERE 1

// Use JUCE's SIMD types
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    using SIMDType = juce::dsp::SIMDRegister<float>;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        int numSamples = buffer.getNumSamples();

        // Process in SIMD chunks
        int numSIMDIterations = numSamples / SIMDType::size();
        for (int i = 0; i < numSIMDIterations; ++i)
        {
            auto simdData = SIMDType::fromRawArray(data);
            simdData *= 0.5f; // Vectorized operation
            simdData.copyToRawArray(data);
            data += SIMDType::size();
        }

        // Handle remaining samples
        for (int i = numSIMDIterations * SIMDType::size(); i < numSamples; ++i)
            data[i] *= 0.5f;
    }
}
```

### 2. Avoid Denormals

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals; // Flush denormals to zero
    // ... processing ...
}
```

### 3. Optimize Memory Access Patterns

```cpp
// Good: Sequential memory access (cache-friendly)
for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
{
    float* data = buffer.getWritePointer(ch);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
        data[i] = process(data[i]);
}

// Bad: Jumping between channels (cache-unfriendly)
for (int i = 0; i < buffer.getNumSamples(); ++i)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.setSample(ch, i, process(buffer.getSample(ch, i)));
}
```

### 4. Pre-Calculate Expensive Operations

```cpp
void prepareToPlay(double sampleRate, int) override
{
    // Pre-calculate lookup table
    constexpr int tableSize = 1024;
    sineTable.resize(tableSize);
    for (int i = 0; i < tableSize; ++i)
        sineTable[i] = std::sin(2.0 * juce::MathConstants<double>::pi * i / tableSize);
}

void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
{
    // Use lookup instead of std::sin() in real-time
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        int index = static_cast<int>(phase * sineTable.size()) % sineTable.size();
        float output = sineTable[index];
        // ...
    }
}
```

---

## Common DSP Algorithms

### 1. Simple Low-Pass Filter (One-Pole)

```cpp
class OnePoleFilter
{
public:
    void setSampleRate(double sr) { sampleRate = sr; }

    void setCutoff(float cutoffHz)
    {
        float omega = 2.0f * juce::MathConstants<float>::pi * cutoffHz / sampleRate;
        a0 = omega / (1.0f + omega);
        b1 = 1.0f - a0;
    }

    float processSample(float input)
    {
        y1 = a0 * input + b1 * y1;
        return y1;
    }

private:
    float a0 = 0.0f, b1 = 0.0f, y1 = 0.0f;
    double sampleRate = 44100.0;
};
```

### 2. Biquad Filter

```cpp
using Biquad = juce::dsp::IIR::Filter<float>;

void setupBiquad(double sampleRate)
{
    // Low-pass filter at 1kHz, Q=0.707
    *biquad.coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, 1000.0, 0.707);
}
```

### 3. Simple Reverb

```cpp
juce::Reverb reverb;

void prepareReverb()
{
    juce::Reverb::Parameters params;
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 0.33f;
    params.dryLevel = 0.67f;
    params.width = 1.0f;
    reverb.setParameters(params);
}

void processWithReverb(juce::AudioBuffer<float>& buffer)
{
    reverb.processStereo(
        buffer.getWritePointer(0),
        buffer.getWritePointer(1),
        buffer.getNumSamples());
}
```

---

## Debugging Audio Code

### 1. Assertions in Debug Builds

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    jassert(buffer.getNumSamples() <= maximumExpectedSamplesPerBlock);
    jassert(!std::isnan(buffer.getSample(0, 0)));
    jassert(!std::isinf(buffer.getSample(0, 0)));
}
```

### 2. Audio Metering

```cpp
class Processor
{
    std::atomic<float> currentLevel{0.0f};

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
    {
        float level = buffer.getMagnitude(0, 0, buffer.getNumSamples());
        currentLevel.store(level);
    }

    float getLevelForGUI() const { return currentLevel.load(); }
};
```

### 3. Offline Rendering for Testing

```cpp
void testProcessing()
{
    MyProcessor processor;
    processor.prepareToPlay(44100.0, 512);

    juce::AudioBuffer<float> testBuffer(2, 512);
    testBuffer.clear();

    // Generate test signal
    for (int i = 0; i < 512; ++i)
        testBuffer.setSample(0, i, std::sin(2.0 * juce::MathConstants<float>::pi * i / 512.0f));

    juce::MidiBuffer emptyMidi;
    processor.processBlock(testBuffer, emptyMidi);

    // Analyze output
    float outputLevel = testBuffer.getMagnitude(0, 0, 512);
    DBG("Output level: " << outputLevel);
}
```

---

## Best Practices Summary

### Do's ✅

1. **Always clear unused output channels** in `processBlock()`
2. **Use `prepareToPlay()`** to allocate resources, calculate coefficients
3. **Use `releaseResources()`** to free memory, close files
4. **Prefer JUCE DSP modules** over custom implementations when possible
5. **Use parameter smoothing** to avoid clicks/pops
6. **Validate audio data** (check for NaN, Inf, denormals)
7. **Profile your code** - measure, don't guess
8. **Test at various sample rates and block sizes**
9. **Support mono, stereo, and multi-channel** configurations
10. **Document CPU usage** of your algorithms

### Don'ts ❌

1. **Never allocate memory** in `processBlock()`
2. **Never use mutexes** in real-time code (use lock-free structures)
3. **Never call system APIs** (file I/O, network, etc.) in audio thread
4. **Don't assume fixed block size** - always query `getNumSamples()`
5. **Don't hardcode sample rate** - use the value from `prepareToPlay()`
6. **Never update GUI directly** from audio thread
7. **Don't use `std::cout`/`printf`** in `processBlock()`
8. **Never throw exceptions** in real-time code
9. **Don't use `std::shared_ptr`** in audio callback (ref count uses locks)
10. **Never assume channel count** - always query dynamically

---

## Reference Links

### Official JUCE Documentation
- [JUCE API Reference](https://docs.juce.com/master/)
- [AudioProcessor Class](https://docs.juce.com/master/classAudioProcessor.html)
- [AudioBuffer Class](https://docs.juce.com/master/classAudioBuffer.html)
- [DSP Module](https://docs.juce.com/master/group__juce__dsp.html)

### Tutorials
- [JUCE Audio Tutorial Series](https://docs.juce.com/master/tutorial_audio_basics.html)
- [Build a Basic Plugin](https://docs.juce.com/master/tutorial_create_projucer_basic_plugin.html)
- [DSP Introduction](https://docs.juce.com/master/tutorial_dsp_introduction.html)

### Community Resources
- [JUCE Forum](https://forum.juce.com/)
- [JUCE Discord](https://discord.gg/juce)
- [TheAudioProgrammer YouTube](https://www.youtube.com/c/TheAudioProgrammer)

---

## Appendix: JUCE Audio Modules

| Module | Purpose |
|--------|---------|
| `juce_audio_basics` | Core audio types (AudioBuffer, MIDI, etc.) |
| `juce_audio_devices` | Hardware audio I/O management |
| `juce_audio_formats` | Audio file reading/writing (WAV, AIFF, etc.) |
| `juce_audio_processors` | Plugin hosting and AudioProcessor base class |
| `juce_audio_utils` | High-level audio utilities |
| `juce_dsp` | DSP algorithms and processors |

---

**Document Version**: 1.0
**Generated**: 2025-10-27
**Target JUCE Version**: 7.0.12+
**Authors**: Claude Code (Anthropic)

---

## Glossary

- **ADC**: Analog-to-Digital Converter
- **DAC**: Digital-to-Analog Converter
- **DSP**: Digital Signal Processing
- **FFT**: Fast Fourier Transform
- **FIR**: Finite Impulse Response
- **IIR**: Infinite Impulse Response
- **MIDI**: Musical Instrument Digital Interface
- **SIMD**: Single Instruction, Multiple Data
- **VST**: Virtual Studio Technology (plugin format)
- **AU**: Audio Unit (Apple plugin format)
- **AAX**: Avid Audio eXtension (Pro Tools plugin format)
- **Denormal**: Floating-point numbers very close to zero that cause CPU slowdown
- **Block Size**: Number of samples processed per audio callback
- **Sample Rate**: Number of audio samples per second (Hz)
- **Latency**: Time delay between input and output
- **Nyquist Frequency**: Half the sample rate; maximum representable frequency
