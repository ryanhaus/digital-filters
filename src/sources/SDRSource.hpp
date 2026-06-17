#pragma once
#include "../sinks/SignalSink.hpp"
#include <SoapySDR/Constants.h>
#include <complex>
#include <stdexcept>
#include <vector>
#include <string>
#include <SoapySDR/Device.hpp>
#include <SoapySDR/Types.hpp>
#include <SoapySDR/Formats.hpp>

// gets samples from an external USB SDR dongle
// uses SoapySDR: https://github.com/pothosware/SoapySDR (needs to be installed)
class SDRSource
{
public:
    SDRSource(SignalSink& destination, float samplingFrequency, float sdrFrequency, float sdrGain);
    ~SDRSource();

    void processBlock();
    SoapySDR::Device* getSDRDevice();

private:
    SignalSink& destination;

    const static size_t SDR_BUFFER_SIZE = 2 << 16;

    SoapySDR::Device* sdr;
    SoapySDR::Stream* rxStream;

    complex<float> buffer[SDR_BUFFER_SIZE];
};
