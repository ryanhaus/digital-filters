#pragma once
#include <complex>

using std::complex;

/**
 * Represents a complex signal.
 */
template<typename T>
class Signal
{
public:
    virtual ~Signal() = default;

    /**
     * Returns the nth sample of the signal.
     *
     * @param n Sample number
     */
    virtual complex<T> nextSample() = 0;
};
