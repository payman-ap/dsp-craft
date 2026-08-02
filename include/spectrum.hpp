#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include <complex>
#include <vector>

namespace dsp {

using Complex = std::complex<double>;

// ---------------------------------------------------------------------------
// Normalization Helpers
// ---------------------------------------------------------------------------

// Normalizes a 1D vector so the maximum absolute value is 1.0
std::vector<double> normalize(const std::vector<double>& x);

// Normalizes a 2D matrix so the global maximum absolute value is 1.0
std::vector<std::vector<double>> normalize(const std::vector<std::vector<double>>& x);

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

// Computes power spectrum |X[k]|^2
std::vector<double> power(const std::vector<Complex>& X);

// Computes power spectrum in decibels: 10 * log10(|X[k]|^2 + eps)
std::vector<double> power_db(const std::vector<Complex>& X, double eps = 1e-12);

// Complex conjugate
std::vector<Complex> conjugate(const std::vector<Complex>& X);

// Wrapped phase difference between two spectra
std::vector<double> phase_difference(const std::vector<Complex>& current, const std::vector<Complex>& previous);

// Frequency axis
std::vector<double> frequency_bins(size_t fft_size, double sample_rate);

// Bin ↔ frequency conversion
double bin_to_frequency(size_t bin, size_t fft_size, double sample_rate);

size_t frequency_to_bin(double frequency, size_t fft_size, double sample_rate);


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


// ---------------------------------------------------------------------------

// Power spectrum
std::vector<std::vector<double>> power(const std::vector<std::vector<Complex>>& stft);

// Power spectrum in dB
std::vector<std::vector<double>> power_db(const std::vector<std::vector<Complex>>& stft, double eps = 1e-12);

// Complex conjugate
std::vector<std::vector<Complex>> conjugate(const std::vector<std::vector<Complex>>& stft);

// ---------------------------------------------------------------------------
// Spectrograms (Returns only non-redundant positive FFT bins: N/2 + 1)
// ---------------------------------------------------------------------------

// Magnitude Spectrogram
std::vector<std::vector<double>> spectrogram_mag(const std::vector<std::vector<Complex>>& stft);

// Magnitude Spectrogram in dB
std::vector<std::vector<double>> spectrogram_db(const std::vector<std::vector<Complex>>& stft, double eps = 1e-12);

// Power Spectrogram
std::vector<std::vector<double>> spectrogram_power(const std::vector<std::vector<Complex>>& stft);

// Power Spectrogram in dB
std::vector<std::vector<double>> spectrogram_power_db(const std::vector<std::vector<Complex>>& stft, double eps = 1e-12);

// Alias for spectrogram_mag for backward compatibility
std::vector<std::vector<double>> spectrogram(const std::vector<std::vector<Complex>>& stft);

} // namespace dsp

#endif // SPECTRUM_HPP

