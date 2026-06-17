#include "ConsoleSink.hpp"

ConsoleSink::ConsoleSink()
{}

void ConsoleSink::processSample(complex<float> sample)
{
    cout << "n = " << n << ": x(n) = " << sample << "\t(mag: " << abs(sample) << ")" << endl;
    n++;
}
