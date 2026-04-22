#include <iostream>
#include <iomanip>
#include "sources/SinusoidalSource.hpp"
#include "filters/FIRFilter.hpp"
#include "utils/FIRCoefficientCalculator.hpp"
#include "sinks/ConsoleSink.hpp"

int main()
{
    // source: sine wave
    SinusoidalSource<double> source (
        1000000,
        2 * M_PI * 300000,
        0
    );

    // FIR filter
    vector<double> firTaps = FIRCoefficientCalculator<double>::calculateHighPassCoefficients(1000000, 200000, 18);
    FIRFilter<double> firFilter(source, firTaps);

    // console output
    std::cout << std::fixed << std::setprecision(3);
    ConsoleSink<double> sink(firFilter);

    sink.printSamples(0, 25);

    return 0;
}

