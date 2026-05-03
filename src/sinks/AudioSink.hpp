#pragma once
#include "SignalSink.hpp"
#include <SDL3/SDL_audio.h>
#include <stdexcept>
#include <iostream>

/**
 * A sink that plays audio from the given samples. Only considers real part of signal.
 */
class AudioSink : public SignalSink
{
public:
    /**
     * Constructs a signal sink that will play audio from the given signal.
     *
     * @param signal The signal going into the sink.
     * @param sampleRate The sampling rate for the audio
     */
    AudioSink(Signal& signal, int sampleRate);

    /**
     * For audio handling, should be called in event loop. Queues more audio
     * samples, if necessary.
     */
    void handleAudio();

private:
    const static int SAMPLE_BUFFER_SIZE = 2048;
    SDL_AudioSpec spec;
    SDL_AudioStream* stream;
    float sampleBuffer[SAMPLE_BUFFER_SIZE];
};
