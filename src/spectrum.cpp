#include "spectrum.hpp"
#include <cmath>

namespace dsp {

namespace {
constexpr double pi = std::numbers::pi;
constexpr double two_pi = 2.0 * std::numbers::pi;
} // anonymous namespace

// ---------------------------------------------------------------------------
// Single Vector Implementations
// ---------------------------------------------------------------------------

std::vector<double> magnitude(const std::vector<Complex>& X)
{
    std::vector<double> mag(X.size());
    for (std::size_t i = 0; i < X.size(); ++i) {
        mag[i] = std::abs(X[i]);
    }
    return mag;
}

std::vector<double> magnitude_db(const std::vector<Complex>& X, double eps)
{
    std::vector<double> mag_db(X.size());
    for (std::size_t i = 0; i < X.size(); ++i) {
        const double mag = std::abs(X[i]);
        mag_db[i] = 20.0 * std::log10(mag + eps);
    }
    return mag_db;
}

// Single Vector Phase implementation
std::vector<double> phase(const std::vector<Complex>& X, bool unwrap)
{
    std::vector<double> ph(X.size());
    for (std::size_t i = 0; i < X.size(); ++i) {
        ph[i] = std::arg(X[i]);
    }

    if (unwrap) {
        return unwrap_phase(ph);
    }
    return ph;
}

std::vector<Complex> polar_to_complex(const std::vector<double>& mag,
                                     const std::vector<double>& phase)
{
    const std::size_t N = std::min(mag.size(), phase.size());
    std::vector<Complex> X(N);
    for (std::size_t i = 0; i < N; ++i) {
        X[i] = std::polar(mag[i], phase[i]);
    }
    return X;
}

// ---------------------------------------------------------------------------
// 2D Matrix Implementations
// ---------------------------------------------------------------------------

std::vector<std::vector<double>> magnitude(const std::vector<std::vector<Complex>>& stft)
{
    std::vector<std::vector<double>> mag_matrix;
    mag_matrix.reserve(stft.size());
    for (const auto& frame : stft) {
        mag_matrix.push_back(magnitude(frame));
    }
    return mag_matrix;
}

std::vector<std::vector<double>> magnitude_db(const std::vector<std::vector<Complex>>& stft,
                                              double eps)
{
    std::vector<std::vector<double>> db_matrix;
    db_matrix.reserve(stft.size());
    for (const auto& frame : stft) {
        db_matrix.push_back(magnitude_db(frame, eps));
    }
    return db_matrix;
}

// 2D STFT Phase implementation
std::vector<std::vector<double>> phase(const std::vector<std::vector<Complex>>& stft, bool unwrap)
{
    std::vector<std::vector<double>> phase_matrix;
    phase_matrix.reserve(stft.size());

    for (const auto& frame : stft) {
        phase_matrix.push_back(phase(frame, unwrap));
    }

    return phase_matrix;
}

std::vector<std::vector<Complex>> polar_to_complex(const std::vector<std::vector<double>>& mag,
                                                  const std::vector<std::vector<double>>& phase)
{
    const std::size_t num_frames = std::min(mag.size(), phase.size());
    std::vector<std::vector<Complex>> stft(num_frames);

    for (std::size_t f = 0; f < num_frames; ++f) {
        stft[f] = polar_to_complex(mag[f], phase[f]);
    }

    return stft;
}

// 1D Phase Unwrap Algorithm
std::vector<double> unwrap_phase(const std::vector<double>& phase_rad)
{
    if (phase_rad.empty()) return {};

    std::vector<double> unwrapped(phase_rad.size());
    unwrapped[0] = phase_rad[0];

    double shift = 0.0;
    for (std::size_t i = 1; i < phase_rad.size(); ++i) {
        double diff = phase_rad[i] - phase_rad[i - 1];

        // Correct jumps larger than +pi or -pi
        while (diff > pi) {
            diff -= two_pi;
            shift -= two_pi;
        }
        while (diff < -pi) {
            diff += two_pi;
            shift += two_pi;
        }

        unwrapped[i] = phase_rad[i] + shift;
    }

    return unwrapped;
}


std::vector<std::vector<double>> spectrogram(const std::vector<std::vector<Complex>>& stft)
{
    auto mag = magnitude(stft);

    for (auto& frame : mag)
        frame.resize(frame.size()/2 + 1);

    return mag;
}



} // namespace dsp

