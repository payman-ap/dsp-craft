#include "windows.hpp"
#include <cmath>
#include <numbers> // C++20 pi constant

namespace dsp {

namespace {
constexpr double pi = std::numbers::pi;
} // anonymous namespace

std::vector<double> rectangular(std::size_t N)
{
    return std::vector<double>(N, 1.0);
}

std::vector<double> hann(std::size_t N, bool periodic)
{
    if (N == 0) return {};
    if (N == 1) return {1.0};

    std::vector<double> win(N);
    const double L = periodic ? static_cast<double>(N) : static_cast<double>(N - 1);

    for (std::size_t i = 0; i < N; ++i) {
        win[i] = 0.5 * (1.0 - std::cos(2.0 * pi * static_cast<double>(i) / L));
    }
    return win;
}

std::vector<double> hamming(std::size_t N, bool periodic)
{
    if (N == 0) return {};
    if (N == 1) return {1.0};

    std::vector<double> win(N);
    const double L = periodic ? static_cast<double>(N) : static_cast<double>(N - 1);

    // Standard Hamming coefficients
    constexpr double a0 = 0.54;
    constexpr double a1 = 0.46;

    for (std::size_t i = 0; i < N; ++i) {
        win[i] = a0 - a1 * std::cos(2.0 * pi * static_cast<double>(i) / L);
    }
    return win;
}

std::vector<double> blackman(std::size_t N, bool periodic)
{
    if (N == 0) return {};
    if (N == 1) return {1.0};

    std::vector<double> win(N);
    const double L = periodic ? static_cast<double>(N) : static_cast<double>(N - 1);

    // Standard Blackman coefficients
    constexpr double a0 = 0.42;
    constexpr double a1 = 0.50;
    constexpr double a2 = 0.08;

    for (std::size_t i = 0; i < N; ++i) {
        const double arg = static_cast<double>(i) / L;
        win[i] = a0 
                 - a1 * std::cos(2.0 * pi * arg) 
                 + a2 * std::cos(4.0 * pi * arg);
    }
    return win;
}

std::vector<double> create_window(std::size_t N, WindowType type, bool periodic)
{
    switch (type) {
        case WindowType::Rectangular:
            return rectangular(N);
        case WindowType::Hann:
            return hann(N, periodic);
        case WindowType::Hamming:
            return hamming(N, periodic);
        case WindowType::Blackman:
            return blackman(N, periodic);
    }
    return {};
}

} // namespace dsp

