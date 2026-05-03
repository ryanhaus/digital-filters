#pragma once
#include "Filter.hpp"

/**
 * Represents a decimator that downsamples a signal
 */
class Decimator : public Filter
{
public:
    /**
     * Constructs a decimator.
     *
     * @param inputSignal The input signal of the filter.
     * @param N The number of input samples per output sample
     * @param offset (optional) How much to offset the first sample by. Default: 0
     */
    Decimator(Signal& inputSignal, size_t N, size_t offset = 0);

    complex<float> nextSample();

private:
    size_t N; //<! The number of input samples per output sample
    size_t offset; //<! How much the first sample is offset by

};
