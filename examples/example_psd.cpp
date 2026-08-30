// examples/example_psd.cpp

#include "fft_iterative.hpp"
#include "spectrum.hpp"
#include "helper_fcns.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>

int main()
{
    // -----------------------------------------------------------------------
    // Parameters
    // -----------------------------------------------------------------------

    constexpr double sample_rate = 1024.0;
    constexpr std::size_t N = 1024;

    constexpr double frequency_1 = 100.0;
    constexpr double frequency_2 = 250.0;

    constexpr double amplitude_1 = 1.0;
    constexpr double amplitude_2 = 0.5;

    // -----------------------------------------------------------------------
    // Generate test signal
    //
    // x[n] = A1*cos(2*pi*f1*n/fs)
    //      + A2*cos(2*pi*f2*n/fs)
    // -----------------------------------------------------------------------

    std::vector<dsp::Complex> signal(N);

    for (std::size_t n = 0; n < N; ++n)
    {
        const double t =
            static_cast<double>(n) / sample_rate;

        signal[n] =
            amplitude_1 *
            std::cos(2.0 * std::numbers::pi * frequency_1 * t)
            +
            amplitude_2 *
            std::cos(2.0 * std::numbers::pi * frequency_2 * t);
    }

    // -----------------------------------------------------------------------
    // FFT
    // -----------------------------------------------------------------------

    auto spectrum = dsp::fft_iterative(signal);

    // -----------------------------------------------------------------------
    // Power Spectral Density
    // -----------------------------------------------------------------------

    auto psd =
        dsp::power_spectral_density(
            spectrum,
            sample_rate);

    auto psd_db =
        dsp::power_spectral_density_db(
            spectrum,
            sample_rate);

    // -----------------------------------------------------------------------
    // Frequency axis
    // -----------------------------------------------------------------------

    auto frequencies =
        dsp::frequency_bins(N, sample_rate);

    // -----------------------------------------------------------------------
    // Save results
    // -----------------------------------------------------------------------

    std::vector<std::vector<double>> psd_data;
    psd_data.reserve(psd.size());

    for (std::size_t k = 0; k < psd.size(); ++k)
    {
        psd_data.push_back({
            frequencies[k],
            psd[k],
            psd_db[k]
        });
    }

    dsp::saveMatrixToCSV(
        "psd.csv",
        psd_data);

    // -----------------------------------------------------------------------
    // Print some information
    // -----------------------------------------------------------------------

    std::cout << "PSD example\n";
    std::cout << "Sample rate: " << sample_rate << " Hz\n";
    std::cout << "FFT size:    " << N << "\n";
    std::cout << "Resolution:  "
              << sample_rate / N
              << " Hz\n";

    std::cout << "PSD bins:    " << psd.size() << "\n";
    std::cout << "Saved to:    psd.csv\n";

    return 0;
}



