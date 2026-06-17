#include "PlotSink.hpp"
#include "implot.h"

PlotSink::PlotSink(std::string plotTitle)
{
    this->plotTitle = plotTitle;
    queuedSamples = std::vector<complex<float>>();
}

void PlotSink::processSample(complex<float> sample)
{
    std::lock_guard<std::mutex> lock(mtx);
    queuedSamples.push_back(sample);
}

void PlotSink::frame()
{
    // process all queued samples from main thread
    std::vector<complex<float>> newSamples;
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (!queuedSamples.empty())
        {
            newSamples = std::move(queuedSamples);
            queuedSamples.clear();
        }
    }

    if (!newSamples.empty())
    {
        for (const auto& sample : newSamples)
        {
            samplesReal.push_back(sample.real());
            samplesImag.push_back(sample.imag());
        }

        // keep only the last maxPlotSamples
        if (samplesReal.size() > maxPlotSamples)
        {
            size_t toErase = samplesReal.size() - maxPlotSamples;
            samplesReal.erase(samplesReal.begin(), samplesReal.begin() + toErase);
            samplesImag.erase(samplesImag.begin(), samplesImag.begin() + toErase);
        }

        // create sample numbers [0, N-1]
        sampleNums.resize(samplesReal.size());
        for (size_t i = 0; i < samplesReal.size(); ++i)
        {
            sampleNums[i] = static_cast<float>(i);
        }
    }

    // imgui
    if (ImPlot::BeginPlot(plotTitle.c_str()))
    {
        ImPlot::SetupAxes("Sample No.", "Value");
        ImPlot::SetupAxesLimits(0, maxPlotSamples, -1.0, 1.0);

        if (!samplesReal.empty())
        {
            ImPlot::PlotLine("Samples (Real)", sampleNums.data(), samplesReal.data(), static_cast<int>(samplesReal.size()));
            ImPlot::PlotLine("Samples (Imag)", sampleNums.data(), samplesImag.data(), static_cast<int>(samplesImag.size()));
        }

        ImPlot::EndPlot();
    }
}
