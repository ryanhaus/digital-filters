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
     * @param start Starting sample number.
     * @param end Ending sample number (included).
     */
    void printSamples(size_t start, size_t end)
    {
        for (size_t n = start; n <= end; n++)
        {
            cout << "n = " << n << ": x(n) = " << this->signal.getSample(n) << endl;
        }
    }
};
