# digital-filters
Experiments with digital filters using a Nooelec RTL-SDR device.

Right now, it's configured to be an FM radio receiver.

## Dependencies
 - [cmake](https://cmake.org/) (and a C++ compiler)
 - [gnuplot](http://www.gnuplot.info/)
 - [SoapySDR](https://github.com/pothosware/SoapySDR)
    - May also require [SoapyRTLSDR](https://github.com/pothosware/SoapyRTLSDR)
 - [SDL3](https://github.com/libsdl-org/SDL)
 - [Doxygen](https://www.doxygen.nl/) (for documentation)

## Compiling
To compile, run the following:

```sh
$ cmake -B bin
$ cmake --build bin -j $(nproc)
```

The generated executable is `./bin/main`.

## Documentation
Documentation can be generated with the following:

```sh
$ doxygen
```

The output will be in the `docs` folder.
