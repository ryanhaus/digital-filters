cmake_minimum_required(VERSION 3.23)
project(digital-filters)

find_package(SDL3 REQUIRED)
find_package(SoapySDR CONFIG REQUIRED)
message("Downloading dependencies")
include(fetch_dependencies.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main)

target_sources(main
    PRIVATE
        src/main.cpp

        src/sources/SinusoidalSource.cpp
        src/sources/SweepSinusoidalSource.cpp
        src/sources/SDRSource.cpp

        src/filters/FIRFilter.cpp
        src/filters/CustomFilter.cpp
        src/filters/FilterPipeline.cpp

        src/sinks/ConsoleSink.cpp
        src/sinks/AudioSink.cpp
        src/sinks/PlotSink.cpp

        src/utils/FIRCoefficientCalculator.cpp)

target_link_libraries(main
    PRIVATE
        SDL3::SDL3
        SoapySDR
        imgui
        implot
)
