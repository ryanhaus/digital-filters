#include "FIRFilter.hpp"

FIRFilter::FIRFilter(SignalSink& destination, vector<float> taps, size_t decimationFactor)
    : Filter(destination),
      taps(taps),
      sampleBuffer(taps.size(), 0),
      writeIndex(0),
      decimationFactor(decimationFactor),
      decimationCounter(0)
{
}

void FIRFilter::processSample(complex<float> sample)
{
    // update sample buffer with new sample
    this->sampleBuffer[writeIndex] = sample;

    // advance write index for next call, but keep it within bounds of buffer
    writeIndex++;
    if (writeIndex >= this->taps.size())
    {
        writeIndex = 0;
    }

    decimationCounter++;

    if (decimationCounter >= decimationFactor)
    {
        // compute convolution
        complex<float> result = 0;
        size_t tapCount = this->taps.size();
        
        // the most recent sample is at (writeIndex - 1 + tapCount) % tapCount
        size_t currentWriteIndex = (writeIndex == 0) ? tapCount - 1 : writeIndex - 1;

        // process samples before currentWriteIndex
        size_t i = 0;
        for (; i <= currentWriteIndex; i++)
        {
            result += this->taps[i] * this->sampleBuffer[currentWriteIndex - i];
        }

        // process samples after currentWriteIndex
        for (; i < tapCount; i++)
        {
            result += this->taps[i] * this->sampleBuffer[currentWriteIndex + tapCount - i];
        }

        // push result downstream
        this->destination.processSample(result);

        // reset decimation counter
        decimationCounter = 0;
    }
}
