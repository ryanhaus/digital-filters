#pragma once
#include "SinusoidalSource.hpp"
#include <cmath>

/**
 * Represents a complex sinusoidal signal that sweeps through frequencies at a given rate.
 */
template<typename T>
class SweepSinusoidalSource : public SinusoidalSource<T>
{
public:
    /**
     * Initializes a complex sinusoidal signal source with a given frequency and phase.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param freqStart The starting frequency in radians per second.
     * @param samplesPerFreq The number of samples spent on each frequency
     * @param deltaFreq How much to increase the frequency by per step
     * @param phase The phase in radians.
     */
    SweepSinusoidalSource<T>(T samplingFreq, T freqStart, size_t samplesPerFreq, T deltaFreq, T phase)
        : SinusoidalSource<T>(samplingFreq, freqStart, phase),
          freqStart(freqStart),
          samplesPerFreq(samplesPerFreq),
          deltaFreq(deltaFreq)
    {}

    complex<T> getSample(size_t n)
    {
        this->freq = freqStart + deltaFreq * (T)(n / samplesPerFreq);

        return SinusoidalSource<T>::getSample(n);
    }

private:
    T freqStart; //!< The starting frequency
    size_t samplesPerFreq; //<! Number of samples spent per frequency step
    T deltaFreq; //<! Increase in frequency per step
};
