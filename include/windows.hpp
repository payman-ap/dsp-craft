// include/windows.hpp
#ifndef WINDOWS_HPP
#define WINDOWS_HPP

#include <iostream>
#include <vector>

namespace dsp {

enum class WindowType
{
    Rectangular,
    Hann,
    Hamming,
    Blackman
};

// Generates a window function of length N.
// If periodic is true, generates an N+1 symmetric window and drops the last sample
// (useful for spectral analysis/STFT overlap-add).
std::vector<double> create_window(std::size_t N, WindowType type, bool periodic = false);

// Specific window generators
std::vector<double> rectangular(std::size_t N);
std::vector<double> hann(std::size_t N, bool periodic = false);
std::vector<double> hamming(std::size_t N, bool periodic = false);
std::vector<double> blackman(std::size_t N, bool periodic = false);

} // namespace dsp

#endif // WINDOWS_HPP
