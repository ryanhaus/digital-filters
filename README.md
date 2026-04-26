# digital-filters
Experiments with digital filters

## Dependencies
 - [cmake](https://cmake.org/) (and a C++ compiler)
 - [gnuplot](http://www.gnuplot.info/)
 - [Matplot++](https://github.com/alandefreitas/matplotplusplus/)
 - [SoapySDR](https://github.com/pothosware/SoapySDR)
 - [SFML 2.6](https://github.com/SFML/SFML)
 - [Doxygen](https://www.doxygen.nl/) (for documentation)

## Compiling
To compile, run the following:

```sh
$ cmake -B bin
$ cmake --build bin
```

The generated executable is `./bin/main`.

## Documentation
Documentation can be generated with the following:

```sh
$ doxygen
```

The output will be in the `docs` folder.
