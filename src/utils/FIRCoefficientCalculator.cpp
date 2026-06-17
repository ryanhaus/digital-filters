#include "FIRCoefficientCalculator.hpp"

float rectangularWindow(int, int)
{
    return 1.0;
}

float hammingWindow(int n, int M)
{
    return 0.54 - 0.46 * cos(2 * M_PI * n / M);
}

float blackmanWindow(int n, int M)
{
    return
        0.42
        - 0.5 * cos(2 * M_PI * n / M)
        + 0.08 * cos(4 * M_PI * n / M);
}

vector<float> FIRCoefficientCalculator::calculateLowPassCoefficients(float samplingFreq, float cutoffFreq, int M, float(*windowFunction)(int, int))
{
    vector<float> taps;
    taps.reserve(M + 1);

    // calculate using sinc function
    float normCutoffFreq = cutoffFreq / samplingFreq;

    for (int n = 0; n <= M; n++)
    {
        float x = (float)2 * normCutoffFreq * (float)(n - M/2);
        float tap = sinc(x);
        taps.push_back(tap * windowFunction(n, M));
    }

    // normalize for unity gain
    float sum = (float)0;
    for (float tap : taps)
    {
        sum += tap;
    }

    for (float& tap : taps)
    {
        tap /= sum;
    }

    return taps;
}

vector<float> FIRCoefficientCalculator::calculateHighPassCoefficients(float samplingFreq, float cutoffFreq, int M, float(*windowFunction)(int, int))
{
    // first, create a lowpass filter
    vector<float> taps = calculateLowPassCoefficients(samplingFreq, cutoffFreq, M, windowFunction);

    // then, do spectral inversion to convert from low-pass to high-pass
    for (float& tap : taps)
    {
        tap *= -1;
    }

    taps[M/2] += 1;

    return taps;
}

vector<float> FIRCoefficientCalculator::calculateBandPassCoefficients(float samplingFreq, float cutoffFreqLow, float cutoffFreqHigh, int M, float(*windowFunction)(int, int))
{
    // create a low pass and a high pass filter with the given frequencies
    vector<float> lowpassTaps = calculateLowPassCoefficients(samplingFreq, cutoffFreqHigh, M, windowFunction);
    vector<float> highpassTaps = calculateHighPassCoefficients(samplingFreq, cutoffFreqLow, M, windowFunction);

    // convolute the taps to create a band pass taps
    vector<float> taps = convolute(lowpassTaps, highpassTaps);
    return taps;
}

float FIRCoefficientCalculator::sinc(float x)
{
    return (x == (float)0)
        ? (float)1
        : sin(M_PI * x) / (M_PI * x);
}

vector<float> FIRCoefficientCalculator::convolute(vector<float> a, vector<float> b)
{
    size_t convolutionN = a.size() + b.size() - 1;
    vector<float> out(convolutionN, 0);

    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < b.size(); j++)
        {
            out[i + j] += a[i] * b[j];
        }
    }

    return out;
}
