#include "sources/SweepSinusoidalSource.hpp"
#include "utils/FIRCoefficientCalculator.hpp"
#include "filters/FIRFilter.hpp"
#include "filters/Decimator.hpp"
#include "sinks/PlotSink.hpp"

int main()
{
    // Example: 100-400 kHz band-pass filter with 1 MHz sampling frequency
    
    // source: sine wave
    SweepSinusoidalSource<double> source (
        1000000,
        2 * M_PI * 0,
        100,
        2 * M_PI * 1000,
        0
    );

    // FIR filter
    vector<double> firTaps =
        FIRCoefficientCalculator<double>::calculateBandPassCoefficients(
            1000000, 
            100000,
            400000,
            40,
            hammingWindow<double>
        );

    FIRFilter<double> firFilter(source, firTaps);
    //for (auto tap : firTaps) std::cout << tap << std::endl;
    
    // decimate transition samples
    Decimator<double> decimator(firFilter, 100, 99);

    // plot output
    PlotSink<double> plot(decimator);
    plot.plotSamples(500, 0.0, 500000.0);

    return 0;
}

