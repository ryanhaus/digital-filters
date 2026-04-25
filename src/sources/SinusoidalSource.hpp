#pragma once
#include "../Signal.hpp"
#include <cmath>

/**
 * Represents a complex sinusoidal signal.
 */
template<typename T>
class SinusoidalSource : public Signal<T>
{
public:
    /**
     * Initializes a complex sinusoidal signal source with a given frequency and phase.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param freq The frequency in radians per second.
     * @param phase The phase in radians.
     */
    SinusoidalSource(T samplingFreq, T freq, T phase)
        : samplingFreq(samplingFreq),
          freq(freq),
          phase(phase),
          n(0)
    {}

    complex<T> nextSample()
    {
        const complex<T> j(0, 1);
        T time = (T)this->n / this->samplingFreq;
        this->n++;

        return exp(j * (this->freq * time + this->phase));
    }

protected:
    T samplingFreq; //!< Sampling frequency of the signal.
    T freq; //!< Frequency of sinusoidal signal in radians per second.
    T phase; //!< Phase shift of sinusoidal signal in radians.
    size_t n; //!< Current sample number
};
