#include "conv.hpp"
#include <iostream>
#include <vector>
#include <cstddef>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>

// ---------------------------------------------------------------------------
// Error metric
// ---------------------------------------------------------------------------

double maxAbsDiff(const std::vector<double>& a, const std::vector<double>& b)
{
    size_t n = std::min(a.size(), b.size());
    double max_diff = 0.0;
    for (size_t i = 0; i < n; ++i) {
        max_diff = std::max(max_diff, std::abs(a[i] - b[i]));
    }
    return max_diff;
}

// ---------------------------------------------------------------------------
// Benchmark
// ---------------------------------------------------------------------------

int main()
{
    constexpr std::size_t N = 4096; // signal length
    constexpr std::size_t M = 256;  // kernel length (naive conv is O(N*M), keep M modest)

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    std::vector<double> x, h;
    x.reserve(N);
    h.reserve(M);
    for (std::size_t i = 0; i < N; ++i) x.push_back(dist(gen));
    for (std::size_t i = 0; i < M; ++i) h.push_back(dist(gen));

    // --- Naive linear convolution ---
    auto start_conv = std::chrono::high_resolution_clock::now();
    auto y_conv = dsp::conv(x, h);
    auto end_conv = std::chrono::high_resolution_clock::now();
    auto duration_conv =
        std::chrono::duration<double, std::micro>(end_conv - start_conv).count();

    // --- Circular convolution, default N (zero-padded to linear length) ---
    auto start_cconv = std::chrono::high_resolution_clock::now();
    auto y_cconv = dsp::cconv(x, h);
    auto end_cconv = std::chrono::high_resolution_clock::now();
    auto duration_cconv =
        std::chrono::duration<double, std::micro>(end_cconv - start_cconv).count();

    // --- FFT-based convolution ---
    auto start_fftconv = std::chrono::high_resolution_clock::now();
    auto y_fftconv = dsp::fftconv(x, h);
    auto end_fftconv = std::chrono::high_resolution_clock::now();
    auto duration_fftconv =
        std::chrono::duration<double, std::micro>(end_fftconv - start_fftconv).count();

    std::cout << "N (signal) = " << N << ", M (kernel) = " << M << "\n\n";

    std::cout << "Naive Conv:  " << duration_conv    << " microseconds\n";
    std::cout << "Circ Conv:   " << duration_cconv   << " microseconds\n";
    std::cout << "FFT Conv:    " << duration_fftconv << " microseconds\n";

    std::cout << "\nError (conv vs fftconv):  " << maxAbsDiff(y_conv, y_fftconv) << "\n";
    std::cout << "Error (conv vs cconv):    " << maxAbsDiff(y_conv, y_cconv)    << "\n";

    return 0;
}