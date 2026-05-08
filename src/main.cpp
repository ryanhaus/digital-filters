#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <implot.h>
#include <stdexcept>
#include <thread>
#include <chrono>
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

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    ImPlot::CreateContext();

    /* referenced: https://www.site2241.net/march2025.htm */
    float samplingFreq = 2.4e6;
    float sdrFreq = 101.5e6;
    float sdrGain = 24.0; // dB

    float fmDeviation = 100e3;

    SDRSource source(samplingFreq, sdrFreq, sdrGain);

    // FIR filter
    vector<float> firTapsLP =
        FIRCoefficientCalculator::calculateLowPassCoefficients(samplingFreq, 100e3, 50, blackmanWindow);

    FIRFilter firFilterLP(source, firTapsLP);
    
    // decimate to 240 kHz to reduce processing
    Decimator decimated240kHz(firFilterLP, 10);

    // compute polar discriminator to find the change in phase
    CustomFilter phaseChange(
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
        FIRCoefficientCalculator::calculateLowPassCoefficients(240e3, 15000, 300, blackmanWindow);

    FIRFilter firFilterDeemph(phaseChange, firTapsDeemph);

    // decimate again to 48 kHz
    Decimator decimated48kHz(firFilterDeemph, 5);

    // play output as audio
    AudioSink audio(decimated48kHz, 48000);

    bool active = true;

    // create thread to handle audio
    std::thread audioThread([&active, &audio]()
        {
            while (active)
            {
                audio.handleAudio();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    );

    // handle video
    while (active)
    {
        // events
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                active = false;
        }

        // imgui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("SDR config");

        ImGui::End();

        // render
        ImGui::Render();

        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // quit
    audioThread.join(); // stops when active == true

    ImPlot::DestroyContext();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

