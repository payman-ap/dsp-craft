#pragma once

#include <vector>
#include <complex>

using Complex = std::complex<double>;

std::vector<Complex> fft(const std::vector<Complex>& x, bool inverse = false, int depth = 0);

