#pragma once
#include <cmath>
#include <vector>

using std::vector;
using std::min;

/**
 * Represents a rectangular window (all values within window are 1)
 */
template<typename T>
T rectangularWindow(int, int)
{
    return (T)1;
}

/**
 * Represents a Hamming window.
 *
 * @param n Value at which to evaluate the window at.
 * @param M The order of the filter.
 */
template<typename T>
T hammingWindow(int n, int M)
{
    return (T)0.54 - (T)0.46 * cos(2 * M_PI * n / M);
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

    /**
     * Computes coefficients/taps for a high-pass FIR filter.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param cutoffFreq The cutoff frequency of the filter.
     * @param M The order of the filter (must be even).
     * @param windowFunction (optional) The window function. Default: rectangular window
     */
    static vector<T> calculateHighPassCoefficients(T samplingFreq, T cutoffFreq, int M, T(*windowFunction)(int, int) = rectangularWindow<T>)
    {
        // First, create a lowpass filter
        vector<T> taps = calculateLowPassCoefficients(samplingFreq, cutoffFreq, M, windowFunction);

        // Then, do spectral inversion to convert from low-pass to high-pass
        for (T& tap : taps)
        {
            tap *= -1;
        }

        taps[M/2] += 1;

        return taps;
    }

    /**
     * Computes coefficients/taps for a high-pass FIR filter.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param cutoffFreqLow The lower cutoff frequency of the filter.
     * @param cutoffFreqHigh The higher cutoff frequency of the filter.
     * @param M The order of the filter (must be even).
     * @param windowFunction (optional) The window function. Default: rectangular window
     */
    static vector<T> calculateBandPassCoefficients(T samplingFreq, T cutoffFreqLow, T cutoffFreqHigh, int M, T(*windowFunction)(int, int) = rectangularWindow<T>)
    {
        // Create a low pass and a high pass filter with the given frequencies
        vector<T> lowpassTaps = calculateLowPassCoefficients(samplingFreq, cutoffFreqHigh, M, windowFunction);
        vector<T> highpassTaps = calculateHighPassCoefficients(samplingFreq, cutoffFreqLow, M, windowFunction);

        // Convolute the taps to create a band pass taps
        vector<T> taps = convolute(lowpassTaps, highpassTaps);
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

    /**
     * Calculates the convolution of two signals
     */
    static vector<T> convolute(vector<T> a, vector<T> b)
    {
        size_t convolutionN = a.size() + b.size() - 1;
        vector<T> out(convolutionN, (T)0);

        for (size_t i = 0; i < a.size(); i++)
        {
            for (size_t j = 0; j < b.size(); j++)
            {
                out[i + j] += a[i] * b[j];
            }
        }

        return out;
    }
};
