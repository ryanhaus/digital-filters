#pragma once
#include "Filter.hpp"
#include <functional>

/**
 * A filter that takes in a custom lambda function, allowing for easy custom filters.
 */
template<typename T>
class CustomFilter : public Filter<T>
{
public:
    /**
     * Creates a custom filter with a given lambda function.
     *
     * @param inputSignal The input signal to the filter.
     * @param func The filter's function, takes in the current sample and returns the filtered sample.
     */
    CustomFilter(Signal<T>& inputSignal, std::function<complex<T>(complex<T>)> func)
        : Filter<T>(inputSignal),
          func(func)
    {}

    complex<T> nextSample()
    {
        complex<T> currentSample = this->inputSignal.nextSample();
        return this->func(currentSample);
    }
private:
    std::function<complex<T>(complex<T>)> func;
};
