#pragma once
#include <complex>

using std::complex;

class SignalSink
{
public:
    virtual ~SignalSink() = default;
    virtual void processSample(complex<float> sample) = 0;
};
