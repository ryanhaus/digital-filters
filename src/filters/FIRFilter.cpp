#include "FIRFilter.hpp"

FIRFilter::FIRFilter(Signal& inputSignal, vector<float> taps)
    : Filter(inputSignal),
      taps(taps),
      sampleBuffer(taps.size(), 0),
      writeIndex(0)
{
}

complex<float> FIRFilter::nextSample()
{
    // update sample buffer with new sample
    this->sampleBuffer[writeIndex] = this->inputSignal.nextSample();

    // compute sample
    complex<float> result = 0;
    size_t tapCount = this->taps.size();

    // process samples before writeIndex
    size_t i = 0;
    for (; i <= writeIndex; i++)
    {
        result += this->taps[i] * this->sampleBuffer[writeIndex - i];
    }

    // process samples after writeIndex
    for (; i < tapCount; i++)
    {
        result += this->taps[i] * this->sampleBuffer[writeIndex + tapCount - i];
    }

    // advance write index for next call, but keep it within bounds of buffer
    writeIndex++;

    if (writeIndex >= tapCount)
    {
        writeIndex = 0;
    }

    return result;
}
