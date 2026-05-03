#pragma once
#include "../Signal.hpp"
#include <cmath>

/**
 * Represents a complex sinusoidal signal.
 */
class SinusoidalSource : public Signal
{
public:
    /**
     * Initializes a complex sinusoidal signal source with a given frequency and phase.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param freq The frequency in radians per second.
     * @param phase The phase in radians.
     */
    SinusoidalSource(float samplingFreq, float freq, float phase);

    complex<float> nextSample();

protected:
    float samplingFreq; //!< Sampling frequency of the signal.
    float freq; //!< Frequency of sinusoidal signal in radians per second.
    float phase; //!< Phase shift of sinusoidal signal in radians.
    size_t n; //!< Current sample number
};
