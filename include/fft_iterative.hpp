// include/fft_iterative.hpp
#ifndef FFT_ITERATIVE_HPP
#define FFT_ITERATIVE_HPP

#include <vector>
#include <complex>

using Complex = std::complex<double>;

void bit_reverse(std::vector<Complex>& x);
void bit_reverse_adv(std::vector<Complex>& x);
std::vector<Complex> fft_iterative(const std::vector<Complex>& signal);
std::vector<Complex> ifft_iterative(const std::vector<Complex>& X);

#endif // FFT_ITERATIVE_HPP