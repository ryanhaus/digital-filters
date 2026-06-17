#include "SinusoidalSource.hpp"

SinusoidalSource::SinusoidalSource(SignalSink& destination, float samplingFreq, float freq, float phase)
    : destination(destination),
      samplingFreq(samplingFreq),
      freq(freq),
      phase(phase),
      n(0)
{}

void SinusoidalSource::generate(size_t nSamples)
{
    for (size_t i = 0; i < nSamples; i++)
    {
        this->destination.processSample(this->nextSample());
    }
}

complex<float> SinusoidalSource::nextSample()
{
    const complex<float> j(0, 1);
    float time = (float)this->n / this->samplingFreq;
    this->n++;

    return exp(j * (this->freq * time + this->phase));
}
