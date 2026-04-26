#pragma once
#include "SignalSink.hpp"
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>

/**
 * A sink that plays audio from the given samples. Only considers real part of signal.
 */
template<typename T>
class AudioSink : public SignalSink<T>, public sf::SoundStream
{
public:
    /**
     * Constructs a signal sink that will play audio from the given signal.
     *
     * @param signal The signal going into the sink.
     * @param sampleRate The sampling rate for the audio
     */
    AudioSink(Signal<T>& signal, unsigned int sampleRate)
        : SignalSink<T>(signal)
    {
        initialize(1, sampleRate);
        samples.reserve(SAMPLE_BUFFER_SIZE);
    }

private:
    const int SAMPLE_BUFFER_SIZE = 4000;
    std::vector<int16_t> samples;
    size_t currentSample;

    /**
     * Converts a complex<T> sample into a 16-bit signed integer to be played as
     * audio. Only considers the real part of the signal, and maps [-1.0, 1.0]
     * to [-32768, 32767], clamping if necessary.
     *
     * @param sample The sample to be converted.
     */
    int16_t convertToInt16(complex<T> sample)
    {
        T x;

        // clamp to [-1.0, 1.0]
        x = std::clamp(sample.real(), (T)-1, (T)1);

        // add 1 and divide by 2 to change range to [0.0, 1.0]
        x = (x + (T)1) / (T)2;

        // multiply by 65535 and convert to integer to change range to [0, 65535]
        x *= (T)65535;
        int converted = (int)round(x);

        // subtract 32768 to convert range to [-32768, 32767]
        converted -= 32768;

        return (int16_t)converted;
    }

    void loadSamples()
    {
        for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++)
        {
            complex<T> currentSample = this->signal.nextSample();
            samples[i] = convertToInt16(currentSample);
        }
    }

    bool onGetData(Chunk& data)
    {
        loadSamples();
        data.sampleCount = SAMPLE_BUFFER_SIZE;
        data.samples = &samples[0];

        return true;
    }

    void onSeek(sf::Time)
    { /* realtime stream, do nothing */ }
};
