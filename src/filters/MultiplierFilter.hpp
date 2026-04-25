#pragma once
#include "Filter.hpp"

/*
 * Multiplies an input signal by a given constant.
 */
template<typename T>
class MultiplierFilter : public Filter<T>
{
public:
    /*
     * Constructs a filter that multiplies an input signal by a given constant.
     * 
     * @param inputSignal The input signal of the filter.
     * @param c The constant to multiply the input signal by.
     * @param realImagIndependent If true, multiplies the real and imaginary components
     *                            independently based on the real and imaginary
     *                            components of 'c'.
     */
    MultiplierFilter(Signal<T>& inputSignal, complex<T> c, bool realImagIndepdenent = false)
        : Filter<T>(inputSignal),
          c(c),
          realImagIndependent(realImagIndepdenent)
    {}

    complex<T> nextSample()
    {
        complex<T> sample = this->inputSignal.nextSample();

        if (realImagIndependent)
        {
            T real = this->c.real() * sample.real();
            T imag = this->c.imag() * sample.imag();

            return std::complex<T>(real, imag);
        }
        else
        {
            return this->c * sample;
        }
    }

private:
    complex<T> c;
    bool realImagIndependent;
};
