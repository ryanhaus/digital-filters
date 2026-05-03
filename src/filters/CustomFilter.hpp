#pragma once
#include "Filter.hpp"
#include <functional>

/**
 * A filter that takes in a custom lambda function, allowing for easy custom filters.
 */
class CustomFilter : public Filter
{
public:
    /**
     * Creates a custom filter with a given lambda function.
     *
     * @param inputSignal The input signal to the filter.
     * @param func The filter's function, takes in the current sample and returns the filtered sample.
     */
    CustomFilter(Signal& inputSignal, std::function<complex<float>(complex<float>)> func);

    complex<float> nextSample();
private:
    std::function<complex<float>(complex<float>)> func;
};
