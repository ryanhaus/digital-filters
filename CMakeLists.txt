cmake_minimum_required(VERSION 3.23)
project(digital-filters)

find_package(SDL3 REQUIRED)
find_package(SoapySDR CONFIG REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main)

target_sources(main
    PRIVATE
        src/main.cpp
        src/utils/FIRCoefficientCalculator.cpp
)

target_link_libraries(main
    PRIVATE
        SDL3::SDL3
        SoapySDR
)
