#include "signals.hpp"
#include <cmath>
#include <numbers>
#include <random>

namespace dsp {

namespace {
constexpr double pi = std::numbers::pi;

// Helper to calculate total sample count safely
std::size_t calculate_num_samples(double fs, double duration)
{
    if (fs <= 0.0 || duration <= 0.0) return 0;
    return static_cast<std::size_t>(std::ceil(fs * duration));
}
} // anonymous namespace

std::vector<double> generate_sine(double freq,
                                  double fs,
                                  double duration,
                                  double amplitude,
                                  double phase_rad)
{
    const std::size_t N = calculate_num_samples(fs, duration);
    std::vector<double> signal(N);

    const double omega = 2.0 * pi * freq / fs;

    for (std::size_t n = 0; n < N; ++n) {
        signal[n] = amplitude * std::sin(omega * static_cast<double>(n) + phase_rad);
    }

    return signal;
}

std::vector<double> generate_chirp(double f0,
                                   double f1,
                                   double fs,
                                   double duration,
                                   double amplitude)
{
    const std::size_t N = calculate_num_samples(fs, duration);
    std::vector<double> signal(N);

    // Linear chirp rate k = (f1 - f0) / T
    const double k = (f1 - f0) / duration;

    for (std::size_t n = 0; n < N; ++n) {
        const double t = static_cast<double>(n) / fs;
        const double phase = 2.0 * pi * (f0 * t + 0.5 * k * t * t);
        signal[n] = amplitude * std::sin(phase);
    }

    return signal;
}

std::vector<double> generate_square(double freq,
                                    double fs,
                                    double duration,
                                    double amplitude,
                                    double duty_cycle)
{
    const std::size_t N = calculate_num_samples(fs, duration);
    std::vector<double> signal(N);

    const double period_samples = fs / freq;

    for (std::size_t n = 0; n < N; ++n) {
        const double phase_in_period = std::fmod(static_cast<double>(n), period_samples) / period_samples;
        signal[n] = (phase_in_period < duty_cycle) ? amplitude : -amplitude;
    }

    return signal;
}

std::vector<double> generate_noise(double fs, double duration, double amplitude)
{
    const std::size_t N = calculate_num_samples(fs, duration);
    std::vector<double> signal(N);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-amplitude, amplitude);

    for (std::size_t n = 0; n < N; ++n) {
        signal[n] = dist(gen);
    }

    return signal;
}

std::vector<double> generate_gaussian_noise(double fs, double duration, double std_dev)
{
    const std::size_t N = calculate_num_samples(fs, duration);
    std::vector<double> signal(N);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, std_dev);

    for (std::size_t n = 0; n < N; ++n) {
        signal[n] = dist(gen);
    }

    return signal;
}

} // namespace dsp

