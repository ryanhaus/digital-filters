#include "SweepSinusoidalSource.hpp"

SweepSinusoidalSource::SweepSinusoidalSource(float samplingFreq, float freqStart, size_t samplesPerFreq, float deltaFreq, float phase)
    : SinusoidalSource(samplingFreq, freqStart, phase),
      freqStart(freqStart),
      samplesPerFreq(samplesPerFreq),
      deltaFreq(deltaFreq)
{}

complex<float> SweepSinusoidalSource::nextSample()
{
    this->freq = freqStart + deltaFreq * ((float)n / samplesPerFreq);

    return SinusoidalSource::nextSample();
}
