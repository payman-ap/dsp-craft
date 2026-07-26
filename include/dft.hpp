// include/dft.hpp
#ifndef DFT_HPP
#define DFT_HPP

#include "helper_fcns.hpp"
#include <vector>
#include <complex>

namespace dsp {

using Complex = std::complex<double>;

// Core DFT function
std::vector<Complex> dft(const std::vector<Complex>& x);

// Utility for comparing spectra
double magnitudeDifference(
    const std::vector<Complex>& a,
    const std::vector<Complex>& b);

} // namespace dsp

#endif // DFT_HPP