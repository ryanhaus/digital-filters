cmake_minimum_required(VERSION 3.23)
project(digital-filters)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main)

target_sources(main
    PRIVATE
        src/main.cpp
)
