#pragma once
#include "SinusoidalSource.hpp"
#include <cmath>

class SweepSinusoidalSource : public SinusoidalSource
{
public:
    SweepSinusoidalSource(SignalSink& destination, float samplingFreq, float freqStart, size_t samplesPerFreq, float deltaFreq, float phase);

    void generate(size_t nSamples) override;

private:
    float freqStart;
    size_t samplesPerFreq;
    float deltaFreq;
};
