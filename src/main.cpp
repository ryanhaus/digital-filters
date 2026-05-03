#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <stdexcept>
#include "sources/SDRSource.hpp"
#include "utils/FIRCoefficientCalculator.hpp"
#include "filters/FIRFilter.hpp"
#include "filters/Decimator.hpp"
#include "filters/CustomFilter.hpp"
#include "sinks/ConsoleSink.hpp"
#include "sinks/AudioSink.hpp"

int main()
{
    // init
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        throw std::runtime_error("Could not initialize SDL");

    // create window
    SDL_Window* window = SDL_CreateWindow("SDL test", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
        throw std::runtime_error("Could not create SDL window: " + std::string(SDL_GetError()));

    // create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
        throw std::runtime_error("Could not create SDL renderer: " + std::string(SDL_GetError()));

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

    // compute polar discriminator to find the change in phase
    CustomFilter<float> phaseChange(
        decimated240kHz,
        [](complex<float> sample)
        {
            static complex<float> prevSamp = 0;

            complex<float> discr = sample * std::conj(prevSamp);
            prevSamp = sample;

            return std::arg(discr);
        }
    );

    // FM deemphasis (approximate)
    vector<float> firTapsDeemph =
        FIRCoefficientCalculator<float>::calculateLowPassCoefficients(240e3, 15000, 300, blackmanWindow);

    FIRFilter<float> firFilterDeemph(phaseChange, firTapsDeemph);

    // decimate again to 48 kHz
    Decimator<float> decimated48kHz(firFilterDeemph, 5);

    // play output as audio
    AudioSink<float> audio(decimated48kHz, 48000);

    bool active = true;
    while (active)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                active = false;
        }

        audio.handleAudio();

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

