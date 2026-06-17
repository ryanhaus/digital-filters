#pragma once
#include "SignalSink.hpp"
#include <SDL3/SDL_audio.h>
#include <stdexcept>
#include <iostream>

// sink that plays samples as audio
class AudioSink : public SignalSink
{
public:
    AudioSink(int sampleRate);
    void processSample(complex<float> sample) override;

private:
    const static int SAMPLE_BUFFER_SIZE = 4096;
    SDL_AudioSpec spec;
    SDL_AudioStream* stream;
    float sampleBuffer[SAMPLE_BUFFER_SIZE];
    int bufferIndex = 0;
};
