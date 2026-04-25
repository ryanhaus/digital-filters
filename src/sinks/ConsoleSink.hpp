#pragma once
#include "SignalSink.hpp"
#include <iostream>

using std::cout;
using std::endl;

template<typename T>
class ConsoleSink : public SignalSink<T>
{
public:
    /**
     * Constructs a signal sink that will print of samples to the console.
     *
     * @param signal The signal going into the sink.
     */
    ConsoleSink<T>(Signal<T>& signal)
    : SignalSink<T>(signal)
    {}

    /**
     * Prints samples over a given range to the console.
     *
     * @param nSamples Number of samples to take.
     */
    void printSamples(size_t nSamples)
    {
        for (size_t n = 0; n < nSamples; n++)
        {
            complex<T> sample = this->signal.nextSample();
            cout << "n = " << n << ": x(n) = " << sample << "\t(mag: " << abs(sample) << ")" << endl;
        }
    }
};
