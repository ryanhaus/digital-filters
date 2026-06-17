#pragma once
#include "Filter.hpp"
#include <vector>

using std::vector;

// FIR filter class
class FIRFilter : public Filter
{
public:
    FIRFilter(vector<float> taps, size_t decimationFactor = 1);
    FIRFilter(SignalSink& destination, vector<float> taps, size_t decimationFactor = 1);
    void processSample(complex<float> sample) override;

private:
    vector<float> taps;
    vector<complex<float>> sampleBuffer;
    size_t writeIndex;
    size_t decimationFactor;
    size_t decimationCounter;
};
