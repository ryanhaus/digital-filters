#pragma once
#include "../Signal.hpp"

using std::complex;

/**
 * Represents a sink for a signal, i.e., something that uses the output of a signal.
 */
template<typename T>
class SignalSink
{
public:
    /**
     * Constructs a signal sink.
     *
     * @param The signal going into the sink.
     */
    SignalSink(Signal<T>& signal)
        : signal(signal)
    {}

protected:
    Signal<T>& signal; //<! The signal going into the sink.
};
