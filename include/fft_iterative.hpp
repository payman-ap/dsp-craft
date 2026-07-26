// include/fft_iterative.hpp
#ifndef FFT_ITERATIVE_HPP
#define FFT_ITERATIVE_HPP

#include <vector>
#include <complex>

namespace dsp {

using Complex = std::complex<double>;

// Public API Functions
std::vector<Complex> fft_iterative(const std::vector<Complex>& signal);
std::vector<Complex> ifft_iterative(const std::vector<Complex>& X);

// Exposed helper algorithms (useful for standalone bit-reversal benchmarks/tests)
void bit_reverse(std::vector<Complex>& x);
void bit_reverse_adv(std::vector<Complex>& x);

} // namespace dsp

#endif // FFT_ITERATIVE_HPP