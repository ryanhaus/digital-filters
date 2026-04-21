#pragma once
#include "Filter.hpp"
#include <vector>

using std::vector;

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
    {}

    complex<T> getSample(size_t n)
    {
        complex<T> result = 0;

        for (size_t i = 0; i < this->taps.size(); i++)
        {
            if (n >= i)
            {
                T tap = this->taps[i];
                complex<T> inputSample = this->inputSignal.getSample(n - i);

                result += tap * inputSample;
            }
        }

        return result;
    }
private:
    vector<T> taps;
};
