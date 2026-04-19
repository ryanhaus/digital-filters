#include "SinusoidalSource.hpp"
#include "ConsoleSink.hpp"

int main()
{
    SinusoidalSource<double> source(1000, 1000, 0);
    ConsoleSink<double> sink(source);

    sink.printSamples(0, 20);

    return 0;
}
