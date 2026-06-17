#pragma once
#include "../sinks/SignalSink.hpp"
#include <cmath>

class SinusoidalSource
{
public:
    virtual ~SinusoidalSource() = default;
    SinusoidalSource(SignalSink& destination, float samplingFreq, float freq, float phase);

    virtual void generate(size_t nSamples);

protected:
    SignalSink& destination;
    float samplingFreq;
    float freq;
    float phase;
    size_t n;

    complex<float> nextSample();
};
