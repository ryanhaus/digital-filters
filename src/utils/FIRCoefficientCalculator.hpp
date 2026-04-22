#pragma once
#include <cmath>
#include <vector>

using std::vector;

/**
 * Represents a rectangular window (all values within window are 1)
 *
 * @param n Value at which to evaluate window at.
 * @param M The order of the filter.
 */
template<typename T>
T rectangularWindow(int n, int M)
{
    return (n >= 0) && (n <= M)
        ? (T)1
        : (T)0;
}

template<typename T>
class FIRCoefficientCalculator
{
public:
    /**
     * Computes coefficients/taps for a low-pass FIR filter.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param cutoffFreq The cutoff frequency of the filter.
     * @param M The order of the filter (must be even).
     * @param windowFunction (optional) The window function. Default: rectangular window
     */
    static vector<T> calculateLowPassCoefficients(T samplingFreq, T cutoffFreq, int M, T(*windowFunction)(int, int) = rectangularWindow<T>)
    {
        vector<T> taps;
        taps.reserve(M + 1);

        // Calculate using sinc function
        T normCutoffFreq = cutoffFreq / samplingFreq;

        for (int n = 0; n <= M; n++)
        {
            T x = (T)2 * normCutoffFreq * (T)(n - M/2);
            T tap = sinc(x);
            taps.push_back(tap * windowFunction(n, M));
        }

        // Normalize for unity gain
        T sum = (T)0;
        for (T tap : taps)
        {
            sum += tap;
        }

        for (T& tap : taps)
        {
            tap /= sum;
        }

        return taps;
    }
    
private:
    /**
     * Calculates sinc(x)
     */
    static T sinc(T x)
    {
        return (x == (T)0)
            ? (T)1
            : sin(M_PI * x) / (M_PI * x);
    }
};
