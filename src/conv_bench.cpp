#include <iostream>
#include <vector>
#include <cstddef>
#include <complex>
#include <random>
#include <chrono>
#include <cmath>
#include <bit>
#include "fft_iterative.hpp"

using Complex = std::complex<double>;

// ---------------------------------------------------------------------------
// Convolution implementations (mirrors conv.cpp)
// ---------------------------------------------------------------------------

std::vector<double> conv(const std::vector<double>& x, const std::vector<double>& h)
{
    size_t N = x.size();
    size_t M = h.size();

    std::vector<double> y(N + M - 1, 0.0);

    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            y[i + j] += x[i] * h[j];
        }
    }

    return y;
}

std::vector<double> cconv(const std::vector<double>& x, const std::vector<double>& h, size_t N = 0)
{
    // If N is 0 or omitted, default to linear convolution length (N_x + N_h - 1)
    size_t min_len = x.size() + h.size() - 1;
    if (N == 0) {
        N = min_len;
    }

    std::vector<double> x_mod(N, 0.0);
    std::vector<double> h_mod(N, 0.0);

    for (size_t i = 0; i < x.size(); ++i) {
        x_mod[i % N] += x[i];
    }
    for (size_t j = 0; j < h.size(); ++j) {
        h_mod[j % N] += h[j];
    }

    std::vector<double> y(N, 0.0);

    for (size_t n = 0; n < N; ++n)
    {
        for (size_t k = 0; k < N; ++k)
        {
            size_t h_idx = (n + N - (k % N)) % N;
            y[n] += x_mod[k] * h_mod[h_idx];
        }
    }

    return y;
}

std::vector<double> fftconv(const std::vector<double>& x, const std::vector<double>& h)
{
    size_t min_len = x.size() + h.size() - 1;
    size_t N = std::bit_ceil(min_len);

    std::vector<Complex> X(N, 0.0);
    std::vector<Complex> H(N, 0.0);
    for (size_t i = 0; i < x.size(); ++i) X[i] = x[i];
    for (size_t i = 0; i < h.size(); ++i) H[i] = h[i];

    std::vector<Complex> X_freq = fft_iterative(X);
    std::vector<Complex> H_freq = fft_iterative(H);

    std::vector<Complex> Y_freq(N);
    for (size_t i = 0; i < N; ++i) {
        Y_freq[i] = X_freq[i] * H_freq[i];
    }

    std::vector<Complex> y_complex = ifft_iterative(Y_freq);

    std::vector<double> y(min_len);
    for (size_t i = 0; i < min_len; ++i) {
        y[i] = y_complex[i].real();
    }

    return y;
}

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
    auto y_conv = conv(x, h);
    auto end_conv = std::chrono::high_resolution_clock::now();
    auto duration_conv =
        std::chrono::duration<double, std::micro>(end_conv - start_conv).count();

    // --- Circular convolution, default N (zero-padded to linear length) ---
    auto start_cconv = std::chrono::high_resolution_clock::now();
    auto y_cconv = cconv(x, h);
    auto end_cconv = std::chrono::high_resolution_clock::now();
    auto duration_cconv =
        std::chrono::duration<double, std::micro>(end_cconv - start_cconv).count();

    // --- FFT-based convolution ---
    auto start_fftconv = std::chrono::high_resolution_clock::now();
    auto y_fftconv = fftconv(x, h);
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

