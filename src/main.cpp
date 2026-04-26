#include "filters/MultiplierFilter.hpp"
#include "filters/SampleDifferentiator.hpp"
#include "sources/SDRSource.hpp"
#include "utils/FIRCoefficientCalculator.hpp"
#include "filters/FIRFilter.hpp"
#include "filters/Decimator.hpp"
#include "filters/ComplexToArg.hpp"
#include "filters/SampleDifferentiator.hpp"
#include "filters/CustomFilter.hpp"
#include "sinks/PlotSink.hpp"
#include "sinks/ConsoleSink.hpp"
#include "sinks/AudioSink.hpp"

int main()
{
    /* referenced: https://www.site2241.net/march2025.htm */
    // tune to FM 89.7 MHz
    float samplingFreq = 2.4e6;
    float sdrFreq = 89.7e6;
    float sdrGain = 24.0; // dB

    float fmDeviation = 100e3;

    SDRSource<float> source(samplingFreq, sdrFreq, sdrGain);

    // FIR filter
    vector<float> firTapsLP =
        FIRCoefficientCalculator<float>::calculateLowPassCoefficients(samplingFreq, 100e3, 50, blackmanWindow);

    FIRFilter<float> firFilterLP(source, firTapsLP);
    
    // decimate to 240 kHz to reduce processing
    Decimator<float> decimated240kHz(firFilterLP, 10);

    // convert from complex samples to arg (arctan), note that the signal should now be real-only
    ComplexToArg<float> arg(decimated240kHz);

    // take derivative of phase to demodulate FM
    SampleDifferentiator<float> differentiator(arg);

    // restrain within boundaries of [-pi, pi]
    CustomFilter<float> modulus(
        differentiator,
        [](complex<float> sample)
        {
            return fmod(sample.real() + M_PI, 2 * M_PI) - M_PI;
        }
    );

    // scale appropriately to match expected amplitude
    MultiplierFilter<float> mult(modulus, 1 / M_PI);//2 * fmDeviation / (samplingFreq/10));

    // FM deemphasis (approximate)
    vector<float> firTapsDeemph =
        FIRCoefficientCalculator<float>::calculateLowPassCoefficients(240e3, 15000, 300, blackmanWindow);

    FIRFilter<float> firFilterDeemph(mult, firTapsDeemph);

    // decimate again to 48 kHz
    Decimator<float> decimated48kHz(firFilterDeemph, 5);

    // play output as audio
    AudioSink<float> audio(decimated48kHz, 48000);

    audio.play();
    while(audio.getStatus() == sf::SoundStream::Playing)
        sf::sleep(sf::seconds(0.1f));

    return 0;
}

