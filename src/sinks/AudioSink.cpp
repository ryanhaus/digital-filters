#include "AudioSink.hpp"

AudioSink::AudioSink(Signal& signal, int sampleRate)
    : SignalSink(signal)
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
void AudioSink::handleAudio()
{
    while (SDL_GetAudioStreamQueued(stream) < sizeof(sampleBuffer))
    {
        for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++)
        {
            complex<float> sample = this->signal.nextSample();
            sampleBuffer[i] = 0.25 * (float)sample.real();
        }

        SDL_PutAudioStreamData(stream, sampleBuffer, sizeof(sampleBuffer));
    }
}
