// examples/benchmark_fft.cpp
#include "dft.hpp"
#include "fft.hpp"
#include "fft_iterative.hpp"
#include <iostream>
#include <random>
#include <chrono>

int main()
{
    constexpr std::size_t N = 4096;

    // Random generator: real and imaginary parts in [-1.0, 1.0]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    std::vector<dsp::Complex> signal;
    signal.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        double re = dist(gen);
        double im = dist(gen);
        signal.emplace_back(re, im);
    }
    
    auto start_fft = std::chrono::high_resolution_clock::now();
    auto fft_result = dsp::fft(signal);
    auto end_fft = std::chrono::high_resolution_clock::now();
    auto duration_fft =
        std::chrono::duration<double, std::micro>
        (end_fft-start_fft)
        .count();

    auto start_dft = std::chrono::high_resolution_clock::now();
    auto dft_result = dsp::dft(signal);
    auto end_dft = std::chrono::high_resolution_clock::now();
    auto duration_dft =
        std::chrono::duration<double, std::micro>
        (end_dft-start_dft)
        .count();

    auto start_ffti = std::chrono::high_resolution_clock::now();
    auto ffti_result = dsp::fft_iterative(signal);
    auto end_ffti = std::chrono::high_resolution_clock::now();
    auto duration_ffti =
        std::chrono::duration<double, std::micro>
        (end_ffti-start_ffti)
        .count();

    std::cout << "FFT: " << duration_fft << " microseconds\n";
    std::cout << "DFT: " << duration_dft << " microseconds\n";
    std::cout << "FFT Iterative: " << duration_ffti << " microseconds\n";

    std::cout 
        << "Error = "
        << dsp::magnitudeDifference(fft_result, dft_result)
        << "\n";

    return 0;
}

