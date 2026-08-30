// examples/example_psd_parseval.cpp

#include "fft_iterative.hpp"
#include "spectrum.hpp"
#include "helper_fcns.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <iomanip>

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
    // Calculate average power directly in the time domain
    //
    // P_x = (1/N) * sum |x[n]|^2
    // -----------------------------------------------------------------------

    double time_domain_power = 0.0;

    for (const auto& sample : signal)
    {
        time_domain_power += std::norm(sample);
    }

    time_domain_power /= static_cast<double>(N);

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

    // -----------------------------------------------------------------------
    // Frequency resolution
    // -----------------------------------------------------------------------

    const double delta_f =
        sample_rate / static_cast<double>(N);

    // -----------------------------------------------------------------------
    // Calculate total power from PSD
    //
    // P_x = sum S_xx[k] * delta_f
    // -----------------------------------------------------------------------

    double psd_power = 0.0;

    for (const double value : psd)
    {
        psd_power += value * delta_f;
    }

    // -----------------------------------------------------------------------
    // Calculate error
    // -----------------------------------------------------------------------

    const double absolute_error =
        std::abs(time_domain_power - psd_power);

    const double relative_error =
        absolute_error / time_domain_power;

    // -----------------------------------------------------------------------
    // Print results
    // -----------------------------------------------------------------------

    std::cout << std::fixed << std::setprecision(12);

    std::cout << "PSD Parseval Test\n";
    std::cout << "-----------------\n";

    std::cout << "Sample rate:          "
              << sample_rate << " Hz\n";

    std::cout << "FFT size:             "
              << N << "\n";

    std::cout << "Frequency resolution: "
              << delta_f << " Hz\n";

    std::cout << "\n";

    std::cout << "Time-domain power:    "
              << time_domain_power << "\n";

    std::cout << "PSD integrated power: "
              << psd_power << "\n";

    std::cout << "Absolute error:       "
              << absolute_error << "\n";

    std::cout << "Relative error:       "
              << relative_error << "\n";

    std::cout << "Relative error (%):   "
              << relative_error * 100.0 << " %\n";

    // -----------------------------------------------------------------------
    // Save PSD for plotting
    // -----------------------------------------------------------------------

    auto frequencies =
        dsp::frequency_bins(N, sample_rate);

    auto psd_db =
        dsp::power_spectral_density_db(
            spectrum,
            sample_rate);

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
        "psd_parseval.csv",
        psd_data);

    // -----------------------------------------------------------------------
    // Test
    // -----------------------------------------------------------------------

    if (relative_error < 1e-10)
    {
        std::cout << "\n[PASS] PSD normalization agrees with "
                     "time-domain power!\n";
    }
    else
    {
        std::cout << "\n[FAIL] PSD normalization does not agree "
                     "with time-domain power!\n";
        return 1;
    }

    std::cout << "Saved PSD to psd_parseval.csv\n";

    return 0;
}

