#pragma once
#include "SignalSink.hpp"
#include <vector>
#include <string>
#include <mutex>

class PlotSink : public SignalSink
{
public:
    PlotSink(std::string plotTitle);
    void processSample(complex<float> sample) override;
    void frame();

private:
    std::string plotTitle;
    std::vector<complex<float>> queuedSamples;
    std::mutex mtx;

    // Rolling buffers for plotting
    std::vector<float> sampleNums;
    std::vector<float> samplesReal;
    std::vector<float> samplesImag;
    const size_t maxPlotSamples = 1000;
};
