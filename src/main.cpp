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
#include <memory>
#include <atomic>
#include "sources/SDRSource.hpp"
#include "utils/FIRCoefficientCalculator.hpp"
#include "filters/FIRFilter.hpp"
#include "filters/CustomFilter.hpp"
#include "filters/FilterPipeline.hpp"
#include "sinks/AudioSink.hpp"
#include "sinks/PlotSink.hpp"

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
    auto volume = std::make_shared<std::atomic<float>>(0.25f);

    // taps for initial lowpass (100 kHz)
    vector<float> firTapsLP =
        FIRCoefficientCalculator::calculateLowPassCoefficients(samplingFreq, 100e3, 50, blackmanWindow);

    // taps for FM deemphasis (15 kHz lowpass)
    vector<float> firTapsDeemph =
        FIRCoefficientCalculator::calculateLowPassCoefficients(240e3, 15000, 300, blackmanWindow);

    // create filter pipeline
    FilterPipeline filters({
        // initial low-pass filter (isolate one channel)
        std::make_shared<FIRFilter>(firTapsLP, 10),

        // compute polar discriminator (change in phase between samples -> scalar)
        std::make_shared<CustomFilter>(
            [](complex<float> sample)
            {
                static complex<float> prevSamp = 0;

                complex<float> discr = sample * std::conj(prevSamp);
                prevSamp = sample;

                return std::arg(discr);
            }
        ),

        // FM deemphasis lowpass filter
        std::make_shared<FIRFilter>(firTapsDeemph, 5),

        // apply volume
        std::make_shared<CustomFilter>(
            [volume](complex<float> sample)
            {
                return sample * volume->load();
            }
        )
    });

    // source signal
    SDRSource source(filters, samplingFreq, sdrFreq, sdrGain);

    // audio at 48 kHz
    AudioSink audio(48000);
    filters.attachTo(audio);

    // plot of audio
    PlotSink audioPlot("Audio Plot");
    filters.attachTo(audioPlot);

    // create thread to drive the pipeline from the source
    bool active = true;

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

        static float volumeVal = 0.25f;
        if (ImGui::SliderFloat("Volume", &volumeVal, 0.0f, 1.0f, "%.2f"))
        {
            volume->store(volumeVal);
        }

        ImGui::End();

        ImGui::Begin("Plots");
        audioPlot.frame();
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
