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
     * @param sampling_freq The sampling frequency of the signal.
     * @param freq The frequency in radians per second.
     * @param phase The phase in radians.
     */
    SinusoidalSource<T>(T sampling_freq, T freq, T phase)
        : sampling_freq(sampling_freq),
          freq(freq),
          phase(phase)
    {}

    complex<T> getSample(size_t n)
    {
        const complex<T> j(0, 1);
        T time = (T)n / this->sampling_freq;

        return exp(j * (this->freq * time + this->phase));
    }

private:
    T sampling_freq; //!< Sampling frequency of the signal.
    T freq; //!< Frequency of sinusoidal signal in radians per second.
    T phase; //!< Phase shift of sinusoidal signal in radians.
};
