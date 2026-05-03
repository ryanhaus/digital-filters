#pragma once
#include "../Signal.hpp"

/*
 * Represents a digital filter that takes in a signal and outputs a filtered signal.
 */
class Filter : public Signal
{
public:
    /*
     * Constructs a digital filter.
     *
     * @param inputSignal The input signal of the filter.
     */
    Filter(Signal& inputSignal)
        : inputSignal(inputSignal)
    {}

    virtual ~Filter() = default;

protected:
    Signal& inputSignal; //<! The input signal of the filter.
};
