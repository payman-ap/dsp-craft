#include "spectrum.hpp"


namespace dsp {

namespace {
constexpr double pi = std::numbers::pi;
constexpr double two_pi = 2.0 * std::numbers::pi;
} // anonymous namespace

// ---------------------------------------------------------------------------
// Normalization Helpers
// ---------------------------------------------------------------------------

std::vector<double> normalize(const std::vector<double>& x)
{
    if (x.empty()) return {};

    double max_val = 0.0;
    for (double val : x) {
        max_val = std::max(max_val, std::abs(val));
    }

    std::vector<double> result(x.size());
    if (max_val < 1e-12) {
        return result; // return zeros if max is near zero
    }

    for (size_t i = 0; i < x.size(); ++i) {
        result[i] = x[i] / max_val;
    }
    return result;
}

std::vector<std::vector<double>> normalize(const std::vector<std::vector<double>>& x)
{
    if (x.empty()) return {};

    double max_val = 0.0;
    for (const auto& frame : x) {
        for (double val : frame) {
            max_val = std::max(max_val, std::abs(val));
        }
    }

    std::vector<std::vector<double>> result(x.size());
    if (max_val < 1e-12) {
        for (size_t f = 0; f < x.size(); ++f) {
            result[f].resize(x[f].size(), 0.0);
        }
        return result;
    }

    for (size_t f = 0; f < x.size(); ++f) {
        result[f].resize(x[f].size());
        for (size_t i = 0; i < x[f].size(); ++i) {
            result[f][i] = x[f][i] / max_val;
        }
    }
    return result;
}


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

std::vector<double> power(const std::vector<Complex>& X)
{
    std::vector<double> p(X.size());

    for (size_t i = 0; i < X.size(); ++i)
    {
        p[i] = std::norm(X[i]); // avoids abs()*abs() and the square root (less expensive)
    }

    return p;
}

std::vector<double> power_db(const std::vector<Complex>& X,
                             double eps)
{
    std::vector<double> p(X.size());

    for (size_t i = 0; i < X.size(); ++i)
    {
        p[i] = 10.0 * std::log10(std::norm(X[i]) + eps);
    }

    return p;
}

std::vector<Complex> conjugate(const std::vector<Complex>& X)
{
    std::vector<Complex> Y(X.size());

    for (size_t i = 0; i < X.size(); ++i)
    {
        Y[i] = std::conj(X[i]);
    }

    return Y;
}

std::vector<double> phase_difference(const std::vector<Complex>& current,
                 const std::vector<Complex>& previous)
{
    const size_t N = std::min(current.size(), previous.size());

    std::vector<double> delta(N);

    for (size_t i = 0; i < N; ++i)
    {
        double diff = std::arg(current[i]) - std::arg(previous[i]);

        while (diff > pi)
            diff -= two_pi;

        while (diff < -pi)
            diff += two_pi;

        delta[i] = diff;
    }

    return delta;
}

std::vector<double> frequency_bins(size_t fft_size, double sample_rate)
{
    std::vector<double> bins(fft_size / 2 + 1);

    const double resolution = sample_rate / fft_size;

    for (size_t k = 0; k < bins.size(); ++k)
    {
        bins[k] = k * resolution;
    }

    return bins;
}

double bin_to_frequency(size_t bin, size_t fft_size, double sample_rate)
{
    return static_cast<double>(bin) * sample_rate / fft_size;
}

size_t frequency_to_bin(double frequency, size_t fft_size, double sample_rate)
{
    return static_cast<size_t>(std::round(frequency * fft_size / sample_rate));
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

// ---------------------------------------------------------------------------

std::vector<std::vector<double>> power(const std::vector<std::vector<Complex>>& stft)
{
    std::vector<std::vector<double>> result;
    result.reserve(stft.size());

    for (const auto& frame : stft)
    {
        result.push_back(power(frame));
    }

    return result;
}

std::vector<std::vector<double>> power_db(const std::vector<std::vector<Complex>>& stft, double eps)
{
    std::vector<std::vector<double>> result;
    result.reserve(stft.size());

    for (const auto& frame : stft)
    {
        result.push_back(power_db(frame, eps));
    }

    return result;
}

std::vector<std::vector<Complex>> conjugate(const std::vector<std::vector<Complex>>& stft)
{
    std::vector<std::vector<Complex>> result;
    result.reserve(stft.size());

    for (const auto& frame : stft)
    {
        result.push_back(conjugate(frame));
    }

    return result;
}


// ---------------------------------------------------------------------------
// Spectrogram Implementations
// ---------------------------------------------------------------------------

std::vector<std::vector<double>> spectrogram_mag(const std::vector<std::vector<Complex>>& stft)
{
    auto mag = magnitude(stft);
    for (auto& frame : mag) {
        frame.resize(frame.size() / 2 + 1);
    }
    return mag;
}

std::vector<std::vector<double>> spectrogram_db(const std::vector<std::vector<Complex>>& stft, double eps)
{
    auto mag_db = magnitude_db(stft, eps);
    for (auto& frame : mag_db) {
        frame.resize(frame.size() / 2 + 1);
    }
    return mag_db;
}

std::vector<std::vector<double>> spectrogram_power(const std::vector<std::vector<Complex>>& stft)
{
    auto pow_mat = power(stft);
    for (auto& frame : pow_mat) {
        frame.resize(frame.size() / 2 + 1);
    }
    return pow_mat;
}

std::vector<std::vector<double>> spectrogram_power_db(const std::vector<std::vector<Complex>>& stft, double eps)
{
    auto pow_db = power_db(stft, eps);
    for (auto& frame : pow_db) {
        frame.resize(frame.size() / 2 + 1);
    }
    return pow_db;
}

std::vector<std::vector<double>> spectrogram(const std::vector<std::vector<Complex>>& stft)
{
    return spectrogram_mag(stft);
}


std::vector<double> power_spectral_density(const std::vector<Complex>& X, double sample_rate)
{
    if (X.empty())
    return {};
    if (sample_rate <= 0.0)
        throw std::invalid_argument("sample_rate must be positive");

    const std::size_t N = X.size();

    std::vector<double> psd (N / 2 + 1);

    const double scale = 1.0 / (static_cast<double>(N) * sample_rate);

    for (std::size_t k = 0; k <= N / 2; ++k)
    {
        psd[k] = std::norm(X[k]) * scale;

        if (k != 0 && k != N / 2)
        {
            psd[k] *= 2.0;
        }
    }

    return psd;
}

std::vector<double> power_spectral_density_db(const std::vector<Complex>& X, double sample_rate, double eps)
{
    std::vector<double> psd = power_spectral_density(X, sample_rate);

    std::vector<double> psd_db (psd.size());

    for (size_t k = 0; k < psd.size(); ++k)
    {
        psd_db[k] = 10.0 * std::log10(psd[k] + eps);
    }

    return psd_db;
}



} // namespace dsp

