#pragma once
#include "Filter.hpp"

/**
 * A filter that converts a complex sample into its arg (arctan). 
 */
template<typename T>
class ComplexToArg : public Filter<T>
{
public:
    /**
     * Constructs a complex to arg filter
     *
     * @param inputSignal The input signal to the filter.
     */
    ComplexToArg(Signal<T>& inputSignal)
        : Filter<T>(inputSignal)
    {}

    complex<T> nextSample()
    {
        complex<T> inputSample = this->inputSignal.nextSample();
        return std::arg(inputSample);
    }
};
