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

    // audio at 48 kHz
    AudioSink audio(48000);

    // FM deemphasis + decimate by 5 (240 kHz -> 48 kHz)
    vector<float> firTapsDeemph =
        FIRCoefficientCalculator::calculateLowPassCoefficients(240e3, 15000, 300, blackmanWindow);
    FIRFilter firFilterDeemph(audio, firTapsDeemph, 5);

    // polar discriminator
    CustomFilter phaseChange(
        firFilterDeemph,
        [](complex<float> sample)
        {
            static complex<float> prevSamp = 0;

            complex<float> discr = sample * std::conj(prevSamp);
            prevSamp = sample;

            return std::arg(discr);
        }
    );

    // low pass + decimate by 10 (2.4 MHz -> 240 kHz)
    vector<float> firTapsLP =
        FIRCoefficientCalculator::calculateLowPassCoefficients(samplingFreq, 100e3, 50, blackmanWindow);
    FIRFilter firFilterLP(phaseChange, firTapsLP, 10);

    // source signal
    SDRSource source(firFilterLP, samplingFreq, sdrFreq, sdrGain);

    bool active = true;

    // create thread to drive the pipeline from the source
    std::thread processingThread([&active, &source]()
        {
            while (active)
            {
                source.processBlock();
            }
        }
    );

    // handle video / UI
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

        static float channelMHz = 101.5;
        if (ImGui::SliderFloat("Channel (MHz)", &channelMHz, 88, 108, "%.1f"))
        {
            sdrFreq = channelMHz * 1e6;

            // round to nearest 100kHz channel
            const float roundTo = 100e3;
            sdrFreq = roundTo * round(sdrFreq / roundTo);

            // set SDR freq
            source.getSDRDevice()
                ->setFrequency(SOAPY_SDR_RX, 0, sdrFreq);
        }

        ImGui::End();

        // render
        ImGui::Render();

        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // quit
    active = false; // ensure thread stops if not already
    processingThread.join();

    ImPlot::DestroyContext();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
