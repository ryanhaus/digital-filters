#pragma once
#include "Filter.hpp"
#include <functional>

// filter that takes in custom lambda function
class CustomFilter : public Filter
{
public:
    CustomFilter(std::function<complex<float>(complex<float>)> func);
    CustomFilter(SignalSink& destination, std::function<complex<float>(complex<float>)> func);
    void processSample(complex<float> sample) override;
private:
    std::function<complex<float>(complex<float>)> func;
};
