#include <iostream>
#include <vector>
#include <cstddef>
#include <complex>
#include <numbers>
#include <bit>
#include "fft_iterative.hpp"

using Complex = std::complex<double>;

namespace dsp {

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
        // N = std::bit_ceil(min_len);
        N = min_len;
    }

    // Step 1: Resize/Pad/Alias 'x' and 'h' to length N
    std::vector<double> x_mod(N, 0.0);
    std::vector<double> h_mod(N, 0.0);

    // Copy and fold 'x' into x_mod of size N (handles both zero-padding and aliasing)
    for (size_t i = 0; i < x.size(); ++i) {
        x_mod[i % N] += x[i];
    }

    // Copy and fold 'h' into h_mod of size N
    for (size_t j = 0; j < h.size(); ++j) {
        h_mod[j % N] += h[j];
    }

    std::vector<double> y(N, 0.0);

    for (size_t n = 0; n < N; ++n)
    {
        for (size_t k = 0; k < N; ++k)
        {
            // Modulo arithmetic causes the periodic wrap-around
            size_t h_idx = (n + N - (k % N)) % N;
            y[n] += x_mod[k] * h_mod[h_idx];
        }
    }

    return y;
}


std::vector<double> fftconv(const std::vector<double>& x, const std::vector<double>& h)
{
    // Step 1: linear convolution length (N_x + N_h - 1)
    size_t min_len = x.size() + h.size() - 1;
    size_t N = std::bit_ceil(min_len);
    
    // Step 2: Zero-pad input vectors and convert to std::complex
    std::vector<Complex> X(N, 0.0);
    std::vector<Complex> H(N, 0.0);
    for (size_t i = 0; i < x.size(); ++i) X[i] = x[i];
    for (size_t i = 0; i < h.size(); ++i) H[i] = h[i];

    // Step 3: Compute FFT of both signals
    std::vector<Complex> X_freq = fft_iterative(X);
    std::vector<Complex> H_freq = fft_iterative(H);

    // Step 4: Element-wise multiplication in frequency domain
    std::vector<Complex> Y_freq(N);
    for (size_t i = 0; i < N; ++i) {
        Y_freq[i] = X_freq[i] * H_freq[i];
    }
    
    // Step 5: Inverse FFT to get back to time domain
    std::vector<Complex> y_complex = ifft_iterative(Y_freq);

    // Step 6: Extract real part and trim back to exact linear length
    std::vector<double> y(min_len);
    for (size_t i = 0; i < min_len; ++i) {
        y[i] = y_complex[i].real();
    }

    return y;
}

} // namespace dsp



