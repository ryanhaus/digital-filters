#pragma once
#include "Filter.hpp"
#include <vector>
#include <deque>

using std::vector;
using std::deque;

template<typename T>
class FIRFilter : public Filter<T>
{
public:
    /*
     * Constructs a Finite Impulse Response (FIR) filter from an input signal
     * and a vector of coefficients (taps).
     *
     * @param inputSignal The input signal of the filter.
     * @param taps The taps (coefficients) of the FIR filter.
     */
    FIRFilter(Signal<T>& inputSignal, vector<T> taps)
        : Filter<T>(inputSignal),
          taps(taps)
    {
        // create empty sample buffer with enough samples to match the number of taps
        this->sampleBuffer = deque<complex<T>>();

        for (size_t i = 0; i < taps.size(); i++)
        {
            this->sampleBuffer.push_front((T)0);
        }
    }

    complex<T> nextSample()
    {
        // update sample buffer with new sample
        complex<T> newSample = this->inputSignal.nextSample();
        this->sampleBuffer.pop_back();
        this->sampleBuffer.push_front(newSample);

        // compute sample
        complex<T> result = 0;

        for (size_t i = 0; i < this->taps.size(); i++)
        {
            T tap = this->taps[i];
            complex<T> inputSample = this->sampleBuffer.at(i);

            result += tap * inputSample;
        }

        return result;
    }
private:
    vector<T> taps;
    deque<complex<T>> sampleBuffer;
};
