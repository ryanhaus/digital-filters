#pragma once
#include "SignalSink.hpp"
#include <matplot/matplot.h>

using namespace matplot;
using std::vector;

template<typename T>
class PlotSink : public SignalSink<T>
{
public:
    /**
     * Constructs a signal sink that will plot samples.
     *
     * @param signal The signal going into the sink.
     */
    PlotSink(Signal<T>& signal)
        : SignalSink<T>(signal)
    {
        figure_handle f = figure();
        f->backend()->run_command("unset warnings"); // avoid annoying gnuplot warnings
    }

    /**
     * Plots samples over a given range to the console.
     *
     * @param nSamples Number of samples to take
     * @param startBound The starting bound of the plot
     * @param endBound The starting bound of the plot
     */
    figure_handle plotSamples(size_t nSamples, T startBound, T endBound)
    {
        vector<double> freq = linspace(startBound, endBound, nSamples);
        vector<double> responseReal;
        vector<double> responseImaginary;

        responseReal.reserve(nSamples);
        responseImaginary.reserve(nSamples);

        for (size_t n = 0; n < nSamples; n++)
        {
            complex<T> sample = this->signal.nextSample();
            responseReal.push_back(sample.real());
            responseImaginary.push_back(sample.imag());
        }

        hold(on);
        plot(freq, responseReal)->line_width(2);
        plot(freq, responseImaginary)->line_width(2);
        show();

        return f;
    }

private:
    figure_handle f;
};
