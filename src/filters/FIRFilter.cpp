#include "FIRFilter.hpp"

FIRFilter::FIRFilter(Signal& inputSignal, vector<float> taps)
    : Filter(inputSignal),
      taps(taps)
{
    // create empty sample buffer with enough samples to match the number of taps
    this->sampleBuffer = deque<complex<float>>();

    for (size_t i = 0; i < taps.size(); i++)
    {
        this->sampleBuffer.push_front(0);
    }
}

complex<float> FIRFilter::nextSample()
{
    // update sample buffer with new sample
    complex<float> newSample = this->inputSignal.nextSample();
    this->sampleBuffer.pop_back();
    this->sampleBuffer.push_front(newSample);

    // compute sample
    complex<float> result = 0;

    for (size_t i = 0; i < this->taps.size(); i++)
    {
        float tap = this->taps[i];
        complex<float> inputSample = this->sampleBuffer.at(i);

        result += tap * inputSample;
    }

    return result;
}
