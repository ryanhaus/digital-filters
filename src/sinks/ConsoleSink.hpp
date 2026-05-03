#pragma once
#include "SignalSink.hpp"
#include <iostream>

using std::cout;
using std::endl;

/**
 * A sink that prints samples to the console.
 */
class ConsoleSink : public SignalSink
{
public:
    /**
     * Constructs a signal sink that will print of samples to the console.
     *
     * @param signal The signal going into the sink.
     */
    ConsoleSink(Signal& signal);

    /**
     * Prints samples over a given range to the console.
     *
     * @param nSamples Number of samples to take.
     */
    void printSamples(size_t nSamples);
};
