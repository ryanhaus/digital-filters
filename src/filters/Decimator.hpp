#pragma once
#include "Filter.hpp"

/**
 * Represents a decimator that downsamples a signal
 */
template<typename T>
class Decimator : public Filter<T>
{
public:
    /**
     * Constructs a decimator.
     *
     * @param inputSignal The input signal of the filter.
     * @param N The number of input samples per output sample
     * @param offset How much to offset the first sample by
     */
    Decimator(Signal<T>& inputSignal, size_t N, size_t offset)
        : Filter<T>(inputSignal),
          N(N),
          offset(offset)
    {}

    complex<T> getSample(size_t n)
    {
        size_t newN = n * this->N + this->offset;
        return this->inputSignal.getSample(newN);
    }

private:
    size_t N; //<! The number of input samples per output sample
    size_t offset; //<! How much the first sample is offset by

};
