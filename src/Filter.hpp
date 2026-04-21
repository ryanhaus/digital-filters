#pragma once
#include "Signal.hpp"

/*
 * Represents a digital filter that takes in a signal and outputs a filtered signal.
 */
template<typename T>
class Filter : public Signal<T>
{
public:
    /*
     * Constructs a digital filter.
     *
     * @param inputSignal The input signal of the filter.
     */
    Filter(Signal<T>& inputSignal)
        : inputSignal(inputSignal)
    {}

    virtual ~Filter() = default;

protected:
    Signal<T>& inputSignal; //<! The input signal of the filter.
};
