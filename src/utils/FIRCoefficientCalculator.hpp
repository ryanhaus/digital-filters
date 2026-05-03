#pragma once
#include <cmath>
#include <vector>

using std::vector;
using std::min;

/**
 * Represents a rectangular window (all values within window are 1)
 */
float rectangularWindow(int, int);

/**
 * Represents a Hamming window.
 *
 * @param n Value at which to evaluate the window at.
 * @param M The order of the filter.
 */
float hammingWindow(int n, int M);

/**
 * Represents a Blackman window.
 *
 * @param n Value at which to evaluate the window at.
 * @param M The order of the filter.
 */
float blackmanWindow(int n, int M);

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
    static vector<float> calculateLowPassCoefficients(float samplingFreq, float cutoffFreq, int M, float(*windowFunction)(int, int) = rectangularWindow);

    /**
     * Computes coefficients/taps for a high-pass FIR filter.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param cutoffFreq The cutoff frequency of the filter.
     * @param M The order of the filter (must be even).
     * @param windowFunction (optional) The window function. Default: rectangular window
     */
    static vector<float> calculateHighPassCoefficients(float samplingFreq, float cutoffFreq, int M, float(*windowFunction)(int, int) = rectangularWindow);

    /**
     * Computes coefficients/taps for a high-pass FIR filter.
     *
     * @param samplingFreq The sampling frequency of the signal.
     * @param cutoffFreqLow The lower cutoff frequency of the filter.
     * @param cutoffFreqHigh The higher cutoff frequency of the filter.
     * @param M The order of the filter (must be even).
     * @param windowFunction (optional) The window function. Default: rectangular window
     */
    static vector<float> calculateBandPassCoefficients(float samplingFreq, float cutoffFreqLow, float cutoffFreqHigh, int M, float(*windowFunction)(int, int) = rectangularWindow);
    
private:
    /**
     * Calculates sinc(x)
     */
    static float sinc(float x);

    /**
     * Calculates the convolution of two signals
     */
    static vector<float> convolute(vector<float> a, vector<float> b);
};
