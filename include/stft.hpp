// include/stft.hpp
#ifndef STFT_HPP
#define STFT_HPP

#include <vector>
#include <complex>
#include <cassert>
#include "signals.hpp"
#include "helper_fcns.hpp"
#include "fft_iterative.hpp"
#include "windows.hpp"
#include "spectrum.hpp"


namespace dsp {

using Complex = std::complex<double>;

// Metadata for STFT parameters
struct STFTConfig {
    size_t fft_size;
    size_t hop_size;
    WindowType window;
};

// Container holding metadata + STFT matrix
struct STFTResult {
    STFTConfig config;
    std::vector<std::vector<Complex>> spectra; // 2D Vector
};


std::vector<double> extract_frame( const std::vector<double>& signal, size_t start, size_t window_size );
void apply_window(std::vector<double>& frame, const std::vector<double>& window);


STFTResult stft(const std::vector<double>& signal, size_t window_size, size_t hop_size, WindowType type);




} // namespace dsp

#endif // STFT_HPP
