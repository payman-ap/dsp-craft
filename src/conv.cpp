#include <iostream>
#include <vector>
#include <cstddef>
#include <complex>
#include <numbers>
#include <bit>
#include "fft_iterative.hpp"

using Complex = std::complex<double>;

template <typename T>
void printVector(const std::vector<T>& vec, const std::string& name = "Vector")
{
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

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






int main ()
{
    std::vector<double> x = {1,2,3};
    std::vector<double> h = {1,-1};
    auto y = conv(x, h);
    printVector(y, "CONV");

    // Case 1: Default N (N = 3 + 2 - 1 = 4). Automatically zero-pads!
    // Gives exact linear convolution output.
    auto yc_default = cconv(x, h);
    printVector(yc_default, "CCONV (Default)"); 

    // Case 2: Explicit N = 3 (Unpadded circular convolution)
    // Causes time-domain aliasing (wrapping)
    auto yc_3 = cconv(x, h, 3);
    printVector(yc_3, "CCONV (N=3, Aliased)");

    // Case 3: Explicit N = 6 (Extra zero-padding)
    auto yc_6 = cconv(x, h, 6);
    printVector(yc_6, "CCONV (N=6, Extra Padded)");

    auto yfft = fftconv(x, h);
    printVector(yfft, "FFT CONV");

    return 0;
}


// // Benchmarking
// #include <random>
// #include <chrono>
// #include <cmath>

// // Simple error metric for real-valued vectors (analogous to magnitudeDifference)
// double maxAbsDiff(const std::vector<double>& a, const std::vector<double>& b)
// {
//     size_t n = std::min(a.size(), b.size());
//     double max_diff = 0.0;
//     for (size_t i = 0; i < n; ++i) {
//         max_diff = std::max(max_diff, std::abs(a[i] - b[i]));
//     }
//     return max_diff;
// }

// int main()
// {
//     constexpr std::size_t N = 4096; // signal length
//     constexpr std::size_t M = 256;  // kernel length (naive conv is O(N*M), keep M modest)

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<double> dist(-1.0, 1.0);

//     std::vector<double> x, h;
//     x.reserve(N);
//     h.reserve(M);
//     for (std::size_t i = 0; i < N; ++i) x.push_back(dist(gen));
//     for (std::size_t i = 0; i < M; ++i) h.push_back(dist(gen));

//     // --- Naive linear convolution ---
//     auto start_conv = std::chrono::high_resolution_clock::now();
//     auto y_conv = conv(x, h);
//     auto end_conv = std::chrono::high_resolution_clock::now();
//     auto duration_conv =
//         std::chrono::duration<double, std::micro>(end_conv - start_conv).count();

//     // --- Circular convolution, default N (zero-padded to linear length) ---
//     auto start_cconv = std::chrono::high_resolution_clock::now();
//     auto y_cconv = cconv(x, h);
//     auto end_cconv = std::chrono::high_resolution_clock::now();
//     auto duration_cconv =
//         std::chrono::duration<double, std::micro>(end_cconv - start_cconv).count();

//     // --- FFT-based convolution ---
//     auto start_fftconv = std::chrono::high_resolution_clock::now();
//     auto y_fftconv = fftconv(x, h);
//     auto end_fftconv = std::chrono::high_resolution_clock::now();
//     auto duration_fftconv =
//         std::chrono::duration<double, std::micro>(end_fftconv - start_fftconv).count();

//     std::cout << "Naive Conv:  " << duration_conv    << " microseconds\n";
//     std::cout << "Circ Conv:   " << duration_cconv   << " microseconds\n";
//     std::cout << "FFT Conv:    " << duration_fftconv << " microseconds\n";

//     std::cout << "Error (conv vs fftconv):  " << maxAbsDiff(y_conv, y_fftconv)  << "\n";
//     std::cout << "Error (conv vs cconv):    " << maxAbsDiff(y_conv, y_cconv)    << "\n";

//     return 0;
// }
