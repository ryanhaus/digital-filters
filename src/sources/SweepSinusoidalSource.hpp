#pragma once
#include "SinusoidalSource.hpp"
#include <cmath>

/**
 * Represents a complex sinusoidal signal that sweeps through frequencies at a given rate.
 */
class SweepSinusoidalSource : public SinusoidalSource
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
    SweepSinusoidalSource(float samplingFreq, float freqStart, size_t samplesPerFreq, float deltaFreq, float phase);

    complex<float> nextSample();

private:
    float freqStart; //!< The starting frequency
    size_t samplesPerFreq; //<! Number of samples spent per frequency step
    float deltaFreq; //<! Increase in frequency per step
};
