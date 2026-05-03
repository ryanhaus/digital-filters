#include "SinusoidalSource.hpp"

SinusoidalSource::SinusoidalSource(float samplingFreq, float freq, float phase)
    : samplingFreq(samplingFreq),
      freq(freq),
      phase(phase),
      n(0)
{}

complex<float> SinusoidalSource::nextSample()
{
    const complex<float> j(0, 1);
    float time = (float)this->n / this->samplingFreq;
    this->n++;

    return exp(j * (this->freq * time + this->phase));
}
