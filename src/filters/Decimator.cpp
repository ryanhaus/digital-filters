#include "Decimator.hpp"

Decimator::Decimator(Signal& inputSignal, size_t N, size_t offset)
    : Filter(inputSignal),
      N(N),
      offset(offset)
{}

complex<float> Decimator::nextSample()
{
    complex<float> desiredSample = 0;

    for (size_t i = 0; i < N; i++)
    {
        complex<float> currentSample = this->inputSignal.nextSample();

        if (i == this->offset)
        {
            desiredSample = currentSample;
        }
    }

    return desiredSample;
}
