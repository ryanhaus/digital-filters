#pragma once
#include "../Signal.hpp"
#include <SoapySDR/Constants.h>
#include <complex>
#include <stdexcept>
#include <vector>
#include <string>
#include <SoapySDR/Device.hpp>
#include <SoapySDR/Types.hpp>
#include <SoapySDR/Formats.hpp>

/**
 * Gets samples from a connected SDR device using SoapySDR.
 * https://github.com/pothosware/SoapySDR (needs to be installed)
 */
class SDRSource : public Signal
{
public:
    /**
     * Initializes an SDR source. For now, assumes an RTL-SDR device is connected.
     *
     * @param samplingFrequency The rate at which the SDR device will be configured to sample.
     * @param sdrFrequency The frequency the SDR will be tuned to.
     * @param sdrGain Gain of RX in dB.
     */
    SDRSource(float samplingFrequency, float sdrFrequency, float sdrGain);

    ~SDRSource();

    complex<float> nextSample();

    SoapySDR::Device* getSDRDevice();

private:
    const static size_t SDR_BUFFER_SIZE = 1024;

    SoapySDR::Device* sdr;
    SoapySDR::Stream* rxStream;

    complex<float> buffer[SDR_BUFFER_SIZE];
    int bufferLen, bufferPtr;
};
