#include "CustomFilter.hpp"

CustomFilter::CustomFilter(Signal& inputSignal, std::function<complex<float>(complex<float>)> func)
    : Filter(inputSignal),
      func(func)
{}

complex<float> CustomFilter::nextSample()
{
    complex<float> currentSample = this->inputSignal.nextSample();
    return this->func(currentSample);
}
