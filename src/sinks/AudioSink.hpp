#pragma once
#include "SignalSink.hpp"
#include <SDL3/SDL_audio.h>
#include <stdexcept>
#include <iostream>

/**
 * A sink that plays audio from the given samples. Only considers real part of signal.
 */
template<typename T>
class AudioSink : public SignalSink<T>
{
public:
    /**
     * Constructs a signal sink that will play audio from the given signal.
     *
     * @param signal The signal going into the sink.
     * @param sampleRate The sampling rate for the audio
     */
    AudioSink(Signal<T>& signal, int sampleRate)
        : SignalSink<T>(signal)
    {
        spec = {
            .format = SDL_AUDIO_F32,
            .channels = 1,
            .freq = sampleRate
        };

        stream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
            &spec,
            NULL,
            NULL
        );

        if (!stream)
            throw std::runtime_error("Could not create SDL audio stream: " + std::string(SDL_GetError()));

        SDL_ResumeAudioStreamDevice(stream);
    }

    /**
     * For audio handling, should be called in event loop. Queues more audio
     * samples, if necessary.
     */
    void handleAudio()
    {
        while (SDL_GetAudioStreamQueued(stream) < sizeof(sampleBuffer))
        {
            for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++)
            {
                complex<T> sample = this->signal.nextSample();
                sampleBuffer[i] = (float)sample.real();
            }

            SDL_PutAudioStreamData(stream, sampleBuffer, sizeof(sampleBuffer));
        }
    }

private:
    const static int SAMPLE_BUFFER_SIZE = 2048;
    SDL_AudioSpec spec;
    SDL_AudioStream* stream;
    float sampleBuffer[SAMPLE_BUFFER_SIZE];
};
