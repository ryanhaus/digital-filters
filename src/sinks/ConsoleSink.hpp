#pragma once
#include "SignalSink.hpp"
#include <iostream>

using std::cout;
using std::endl;

class ConsoleSink : public SignalSink
{
public:
    ConsoleSink();
    void processSample(complex<float> sample) override;

private:
    size_t n = 0;
};
