#include "windows.hpp"
#include "helper_fcns.hpp"
#include <iostream>
#include <vector>
#include <numeric>

int main()
{
    constexpr std::size_t N = 64;

    // Generate windows
    auto rect     = dsp::create_window(N, dsp::WindowType::Rectangular);
    auto hann     = dsp::create_window(N, dsp::WindowType::Hann);
    auto hamming  = dsp::create_window(N, dsp::WindowType::Hamming);
    auto blackman = dsp::create_window(N, dsp::WindowType::Blackman);

    // Create sample indices [0, 1, ..., N-1]
    std::vector<double> sample_indices(N);
    std::iota(sample_indices.begin(), sample_indices.end(), 0.0);

    // Print quick console verification
    std::cout << "=== Window Generation Summary (N = " << N << ") ===\n";
    std::cout << "Rectangular mid-value: " << rect[N / 2] << "\n";
    std::cout << "Hann mid-value:        " << hann[N / 2] << "\n";
    std::cout << "Hamming mid-value:     " << hamming[N / 2] << "\n";
    std::cout << "Blackman mid-value:    " << blackman[N / 2] << "\n\n";

    // Export all windows side-by-side to CSV
    std::vector<std::string> headers = {"sample", "rectangular", "hann", "hamming", "blackman"};
    dsp::saveColumnsToCSV("windows_comparison.csv", headers, sample_indices, rect, hann, hamming, blackman);

    dsp::saveToCSV("windows_hann.csv", hann, "");

    return 0;
}

