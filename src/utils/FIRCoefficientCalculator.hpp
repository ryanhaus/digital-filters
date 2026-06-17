#pragma once
#include <cmath>
#include <vector>

using std::vector;
using std::min;

float rectangularWindow(int, int);
float hammingWindow(int n, int M);
float blackmanWindow(int n, int M);

class FIRCoefficientCalculator
{
public:
    static vector<float> calculateLowPassCoefficients(float samplingFreq, float cutoffFreq, int M, float(*windowFunction)(int, int) = rectangularWindow);
    static vector<float> calculateHighPassCoefficients(float samplingFreq, float cutoffFreq, int M, float(*windowFunction)(int, int) = rectangularWindow);
    static vector<float> calculateBandPassCoefficients(float samplingFreq, float cutoffFreqLow, float cutoffFreqHigh, int M, float(*windowFunction)(int, int) = rectangularWindow);
    
private:
    static float sinc(float x);
    static vector<float> convolute(vector<float> a, vector<float> b);
};
