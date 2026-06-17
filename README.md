# digital-filters
Experiments with digital filters using a Nooelec RTL-SDR device.

Right now, it's configured to be an FM radio receiver.

## Dependencies
 - [cmake](https://cmake.org/) (and a C++ compiler)
 - [SoapySDR](https://github.com/pothosware/SoapySDR)
    - May also require [SoapyRTLSDR](https://github.com/pothosware/SoapyRTLSDR)
 - [SDL3](https://github.com/libsdl-org/SDL)
 - [Doxygen](https://www.doxygen.nl/) (for documentation)
 
 The build script also pulls in Imgui and Implot automatically.

## Compiling
To compile, run the following:

```sh
$ cmake -B build
$ cmake --build build -j $(nproc)
```

The generated executable is `./build/main`.
