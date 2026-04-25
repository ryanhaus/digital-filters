#include "sources/SweepSinusoidalSource.hpp"
#include "sources/SDRSource.hpp"
#include "utils/FIRCoefficientCalculator.hpp"
#include "filters/FIRFilter.hpp"
#include "filters/Decimator.hpp"
#include "sinks/PlotSink.hpp"

int main()
{
    // Example: 100-400 kHz band-pass filter with 1 MHz sampling frequency
    float samplingFreq = 3e6;
    float sdrFreq = 89e6;
    SDRSource<float> source(samplingFreq, sdrFreq);

    // FIR filter
    vector<float> firTaps =
        FIRCoefficientCalculator<float>::calculateBandPassCoefficients(
            samplingFreq, 
            600000,
            800000,
            100,
            hammingWindow<float>
        );

    FIRFilter<float> firFilter(source, firTaps);
    //for (auto tap : firTaps) std::cout << tap << std::endl;
    
    // decimate transition samples
    Decimator<float> decimator(firFilter, 100, 0);

    // plot output
    PlotSink<float> plot(firFilter);
    plot.plotSamples(1000, 0.0, 1.0);

    return 0;
}

