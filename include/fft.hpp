// include/fft.hpp
#ifndef FFT_HPP
#define FFT_HPP

#include "helper_fcns.hpp"
#include <vector>
#include <complex>
#include <string>

namespace dsp {

using Complex = std::complex<double>;

// Original helper retained
// void printVector(const std::vector<Complex>& vec, const std::string& name = "Vector");

// Recursive FFT & IFFT
std::vector<Complex> fft(const std::vector<Complex>& x, int depth = 0);
std::vector<Complex> ifft(const std::vector<Complex>& X);

} // namespace dsp

#endif // FFT_HPP