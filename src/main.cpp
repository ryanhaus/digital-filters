#include <iostream>
#include <iomanip>
#include "sources/SinusoidalSource.hpp"
#include "filters/FIRFilter.hpp"
#include "sinks/ConsoleSink.hpp"

int main()
{
    // source: sine wave
    SinusoidalSource<double> source (
        1000000,
        2 * M_PI * 250000,
        0
    );

    // FIR filter
    vector<double> firTaps = { -0.0265, 0, 0.0441, 0, -0.0934, 0, 0.3139, 0.5000, 0.3139, 0, -0.0934, 0, 0.0441, 0, -0.0265 }; // from https://www.hackster.io/whitney-knitter/dsp-for-fpga-simple-fir-filter-in-verilog-91208d
    FIRFilter<double> firFilter(source, firTaps);

    // console output
    std::cout << std::fixed << std::setprecision(3);
    ConsoleSink<double> sink(firFilter);

    sink.printSamples(0, 100);

    return 0;
}

