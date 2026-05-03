#pragma once
#include <complex>

using std::complex;

/**
 * Represents a complex signal.
 */
class Signal
{
public:
    virtual ~Signal() = default;

    /**
     * Returns the nth sample of the signal.
     *
     * @param n Sample number
     */
    virtual complex<float> nextSample() = 0;
};
