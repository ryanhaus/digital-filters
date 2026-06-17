#include "AudioSink.hpp"

AudioSink::AudioSink(int sampleRate)
    : bufferIndex(0)
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

void AudioSink::processSample(complex<float> sample)
{
    sampleBuffer[bufferIndex++] = (float)sample.real();

    if (bufferIndex >= SAMPLE_BUFFER_SIZE)
    {
        SDL_PutAudioStreamData(stream, sampleBuffer, sizeof(sampleBuffer));
        bufferIndex = 0;
    }
}
