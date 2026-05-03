#include "ConsoleSink.hpp"

ConsoleSink::ConsoleSink(Signal& signal)
    : SignalSink(signal)
{}

/**
 * Prints samples over a given range to the console.
 *
 * @param nSamples Number of samples to take.
 */
void ConsoleSink::printSamples(size_t nSamples)
{
    for (size_t n = 0; n < nSamples; n++)
    {
        complex<float> sample = this->signal.nextSample();
        cout << "n = " << n << ": x(n) = " << sample << "\t(mag: " << abs(sample) << ")" << endl;
    }
}
