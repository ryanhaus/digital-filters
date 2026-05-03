#pragma once
#include "../Signal.hpp"

using std::complex;

/**
 * Represents a sink for a signal, i.e., something that uses the output of a signal.
 */
class SignalSink
{
public:
    /**
     * Constructs a signal sink.
     *
     * @param The signal going into the sink.
     */
    SignalSink(Signal& signal)
        : signal(signal)
    {}

protected:
    Signal& signal; //<! The signal going into the sink.
};
