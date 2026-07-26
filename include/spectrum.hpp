#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include <complex>
#include <vector>

namespace dsp {

using Complex = std::complex<double>;

// ---------------------------------------------------------------------------
// Single Vector Transformations
// ---------------------------------------------------------------------------

// Computes magnitude spectrum |X[k]|
std::vector<double> magnitude(const std::vector<Complex>& X);

// Computes magnitude spectrum in decibels: 20 * log10(|X[k]| + eps)
std::vector<double> magnitude_db(const std::vector<Complex>& X, double eps = 1e-12);

// Computes phase spectrum in radians: arg(X[k])
// If unwrap is true, removes 2*pi phase jumps across adjacent bins
std::vector<double> phase(const std::vector<Complex>& X, bool unwrap = false);

// Helper function to unwrap an existing vector of phase values
std::vector<double> unwrap_phase(const std::vector<double>& phase_rad);

// Reconstructs complex spectrum from magnitude and phase: mag * exp(j * phase)
std::vector<Complex> polar_to_complex(const std::vector<double>& mag,
                                     const std::vector<double>& phase);

// ---------------------------------------------------------------------------
// 2D Matrix Transformations (Frame-by-Frame for STFT)
// ---------------------------------------------------------------------------

// Matrix magnitude |STFT[frame][bin]|
std::vector<std::vector<double>> magnitude(const std::vector<std::vector<Complex>>& stft);

// Matrix magnitude in dB
std::vector<std::vector<double>> magnitude_db(const std::vector<std::vector<Complex>>& stft,
                                              double eps = 1e-12);

// Matrix phase arg(STFT[frame][bin])
std::vector<std::vector<double>> phase(const std::vector<std::vector<Complex>>& stft, bool unwrap = false);

// Reconstructs STFT complex matrix from magnitude and phase matrices
std::vector<std::vector<Complex>> polar_to_complex(const std::vector<std::vector<double>>& mag,
                                                  const std::vector<std::vector<double>>& phase);


                                                  // returns only the unique FFT bins
std::vector<std::vector<double>> spectrogram(const std::vector<std::vector<Complex>>& stft);




} // namespace dsp

#endif // SPECTRUM_HPP

