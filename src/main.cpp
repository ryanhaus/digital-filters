#include <iostream>
#include <iomanip>
#include "sources/SweepSinusoidalSource.hpp"
#include "utils/FIRCoefficientCalculator.hpp"
#include "filters/FIRFilter.hpp"
#include "filters/Decimator.hpp"
#include "sinks/ConsoleSink.hpp"

int main()
{
    // source: sine wave
    SweepSinusoidalSource<double> source (
        1000000,
        2 * M_PI * 0,
        100,
        2 * M_PI * 10000,
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

    // console output
    std::cout << std::fixed << std::setprecision(3);
    ConsoleSink<double> sink(decimator);

    sink.printSamples(0, 50);

    return 0;
}

