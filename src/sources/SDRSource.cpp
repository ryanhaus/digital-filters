#include "SDRSource.hpp"

SDRSource::SDRSource(float samplingFrequency, float sdrFrequency, float sdrGain)
    : bufferLen(0),
      bufferPtr(0)
{
    using std::vector;
    using std::string;

    /* reference: https://github.com/pothosware/SoapySDR/wiki/Cpp_API_Example */
    // find SDR devices
    SoapySDR::KwargsList results = SoapySDR::Device::enumerate();
    
    // Go through each SDR device and print info
    size_t i;
    SoapySDR::Kwargs::iterator it;
    for(i = 0; i < results.size(); ++i)
    {
    	printf("Found device: ");
    	for( it = results[i].begin(); it != results[i].end(); ++it)
    	{
    		printf("%s = %s\n", it->first.c_str(), it->second.c_str());
    	}
    	printf("\n");
    }

    if (i == 0)
    {
        throw std::runtime_error("No SDR device found");
    }

    // select first SDR device
    SoapySDR::Kwargs args = results[0];

    sdr = SoapySDR::Device::make(args);
    if (sdr == NULL)
    {
        throw std::runtime_error("SoapySDR::Device::make failed");
    }

    // print device info
    vector<string> str_list;

    str_list = sdr->listAntennas(SOAPY_SDR_RX, 0);
    printf("RX antennas: ");
    for (const string &s : str_list)
    {
        printf("%s, ", s.c_str());
    }
    printf("\n");

    str_list = sdr->listGains(SOAPY_SDR_RX, 0);
    printf("RX gains: ");
    for (const string &s : str_list)
    {
        printf("%s, ", s.c_str());
    }
    printf("\n");

    SoapySDR::RangeList ranges = sdr->getFrequencyRange(SOAPY_SDR_RX, 0);
    printf("Frequency ranges: ");
    for (const SoapySDR::Range &range : ranges)
    {
        printf("[%g Hz -> %g Hz], ", range.minimum(), range.maximum());
    }
    printf("\n");

    // set given settings
    sdr->setGain(SOAPY_SDR_RX, 0, sdrGain);
    sdr->setSampleRate(SOAPY_SDR_RX, 0, samplingFrequency);
    sdr->setFrequency(SOAPY_SDR_RX, 0, sdrFrequency);

    // setup stream
    rxStream = sdr->setupStream(SOAPY_SDR_RX, SOAPY_SDR_CF32);
    if (rxStream == NULL)
    {
        SoapySDR::Device::unmake(sdr);
        throw std::runtime_error("Failed to create stream");
    }

    sdr->activateStream(rxStream, 0, 0, 0);
}

SDRSource::~SDRSource()
{
    sdr->deactivateStream(rxStream, 0, 0);
    sdr->closeStream(rxStream);

    SoapySDR::Device::unmake(sdr);
}

complex<float> SDRSource::nextSample()
{
    // Read from device, if necessary
    if (bufferPtr == bufferLen)
    {
        void* buffs[] = { buffer };
        int flags;
        long long timeNs;
        int ret;

        // Retry on timeout
        for (int retry = 0; retry < 10; retry++)
        {
            ret = sdr->readStream(rxStream, buffs, SDR_BUFFER_SIZE, flags, timeNs);
            if (ret != SOAPY_SDR_TIMEOUT) break;
        }

        if (ret < 0)
        {
            throw std::runtime_error("Error reading from SDR stream: " + std::string(SoapySDR_errToStr(ret)));
        }

        bufferLen = ret;
        bufferPtr = 0;
    }

    // continue through sample buffer
    return buffer[bufferPtr++];
}

SoapySDR::Device* SDRSource::getSDRDevice()
{
    return sdr;
}
