#include <iostream>
#include <iomanip>
#include "SinusoidalSource.hpp"
#include "MultiplierFilter.hpp"
#include "FIRFilter.hpp"
#include "ConsoleSink.hpp"

int main()
{
    // source: sine wave
    SinusoidalSource<double> source (
        1000000,
        2 * M_PI * 200000,
        0
    );

    // Take real component only
    // MultiplierFilter<double> multFilter(source, std::complex<double>(1, 0), true);

    // FIR filter
    vector<double> firTaps = { -0.0265, 0, 0.0441, 0, -0.0934, 0, 0.3139, 0.5000, 0.3139, 0, -0.0934, 0, 0.0441, 0, -0.0265 }; // from https://www.hackster.io/whitney-knitter/dsp-for-fpga-simple-fir-filter-in-verilog-91208d
    FIRFilter<double> firFilter(source, firTaps);

    // console output
    std::cout << std::fixed << std::setprecision(3);
    ConsoleSink<double> sink(firFilter);

    sink.printSamples(0, 100);

    return 0;
}

