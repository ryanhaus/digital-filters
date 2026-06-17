#include "SweepSinusoidalSource.hpp"

SweepSinusoidalSource::SweepSinusoidalSource(SignalSink& destination, float samplingFreq, float freqStart, size_t samplesPerFreq, float deltaFreq, float phase)
    : SinusoidalSource(destination, samplingFreq, freqStart, phase),
      freqStart(freqStart),
      samplesPerFreq(samplesPerFreq),
      deltaFreq(deltaFreq)
{}

void SweepSinusoidalSource::generate(size_t nSamples)
{
    for (size_t i = 0; i < nSamples; i++)
    {
        this->freq = freqStart + deltaFreq * ((float)n / samplesPerFreq);
        this->destination.processSample(this->nextSample());
    }
}
