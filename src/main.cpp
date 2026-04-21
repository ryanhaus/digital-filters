#include <iostream>
#include <iomanip>
#include "SinusoidalSource.hpp"
#include "ConsoleSink.hpp"

int main()
{
    SinusoidalSource<double> source(1000, 1000, 0);
    ConsoleSink<double> sink(source);

    std::cout << std::fixed << std::setprecision(3);
    ConsoleSink<double> sink(firFilter);

    sink.printSamples(0, 100);

    return 0;
}

