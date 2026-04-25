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
    {}

    /**
     * Plots samples over a given range to the console.
     *
     * @param nSamples Number of samples to take
     * @param startBound The starting bound of the plot (defaults to '')
     * @param endBound The starting bound of the plot (defaults to 'end')
     */
    figure_handle plotSamples(size_t nSamples, T startBound, T endBound)
    {
        figure_handle f = figure();
        f->backend()->run_command("unset warnings"); // avoid annoying gnuplot warnings
                                                           
        vector<double> freq = linspace(startBound, endBound, nSamples);
        vector<double> response;
        response.reserve(nSamples);

        for (size_t n = 0; n < nSamples; n++)
        {
            complex<T> sample = this->signal.nextSample();
            response.push_back(abs(sample));
        }

        plot(freq, response)->line_width(2);
        show();

        return f;
    }
private:
};
