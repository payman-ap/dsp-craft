// include/conv.hpp
#ifndef CONV_HPP
#define CONV_HPP

#include <vector>
#include <cstddef>

namespace dsp {

// Time-domain direct linear convolution
std::vector<double> conv(const std::vector<double>& x, const std::vector<double>& h);

// Circular convolution (with optional length N)
std::vector<double> cconv(const std::vector<double>& x, const std::vector<double>& h, size_t N = 0);

// Fast FFT-based linear convolution (zero-padded to next power of 2)
std::vector<double> fftconv(const std::vector<double>& x, const std::vector<double>& h);

} // namespace dsp

#endif // CONV_HPP

