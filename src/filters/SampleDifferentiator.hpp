#pragma once
#include "Filter.hpp"

/**
 * A filter which computes the derivative of the signal which respect to the
 * number of samples by subtracting the current sample with the previous one.
 */
template<typename T>
class SampleDifferentiator : public Filter<T>
{
public:
    /**
     * Constructs a sample differentiator filter
     *
     * @param inputSignal The input signal to the filter.
     */
    SampleDifferentiator(Signal<T>& inputSignal)
        : Filter<T>(inputSignal),
          previousSample(0)
    {}

    complex<T> nextSample()
    {
        // get next sample
        complex<T> currentSample = this->inputSignal.nextSample();

        // compute derivative
        complex<T> difference = currentSample - previousSample;
        previousSample = currentSample;

        return difference;
    }

private:
    complex<T> previousSample;
};
