#include "CustomFilter.hpp"

CustomFilter::CustomFilter(SignalSink& destination, std::function<complex<float>(complex<float>)> func)
    : Filter(destination),
      func(func)
{}

void CustomFilter::processSample(complex<float> sample)
{
    complex<float> result = this->func(sample);
    this->destination.processSample(result);
}
