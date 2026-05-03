#pragma once
#include "Filter.hpp"
#include <vector>
#include <deque>

using std::vector;
using std::deque;

class FIRFilter : public Filter
{
public:
    /*
     * Constructs a Finite Impulse Response (FIR) filter from an input signal
     * and a vector of coefficients (taps).
     *
     * @param inputSignal The input signal of the filter.
     * @param taps The taps (coefficients) of the FIR filter.
     */
    FIRFilter(Signal& inputSignal, vector<float> taps);

    complex<float> nextSample();

private:
    vector<float> taps;
    deque<complex<float>> sampleBuffer;
};
