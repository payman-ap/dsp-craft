#include "spectrum.hpp"
#include "fft.hpp"
#include "stft.hpp"
#include "signals.hpp"
#include "helper_fcns.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

std::vector<dsp::Complex> shift_bins(const std::vector<dsp::Complex>& spectrum, int shift)
{
    const size_t N = spectrum.size();
    std::vector<dsp::Complex> out(N, dsp::Complex{0.0, 0.0});

    for (size_t k = 0; k < N; ++k)
    {
        int new_bin = static_cast<int>(k) + shift;

        if (new_bin >= 0 && new_bin < static_cast<int>(N))
        {
            out[new_bin] = spectrum[k];
        }
    }

    return out;
}

std::vector<dsp::Complex> shift_bins_real(const std::vector<dsp::Complex>& X, int shift)
{
    const size_t N = X.size();

    std::vector<dsp::Complex> Y(N, {0.0, 0.0});

    const size_t nyquist = N / 2;

    // DC stays where it is
    Y[0] = X[0];

    // Shift positive frequencies
    for (size_t k = 1; k < nyquist; ++k)
    {
        int dst = static_cast<int>(k) + shift;

        if (dst > 0 &&
            dst < static_cast<int>(nyquist))
        {
            Y[dst] = X[k];
        }
    }

    // Nyquist
    Y[nyquist] = X[nyquist];

    // Rebuild negative frequencies
    for (size_t k = 1; k < nyquist; ++k)
    {
        Y[N - k] = std::conj(Y[k]);
    }

    return Y;
}

int main()
{
    // 1. Generate signal
    constexpr double fs = 44100.0;
    constexpr double duration = 1.0; 
    auto signal = dsp::generate_chirp(140.0, 20000.0, fs, duration, 1.0);

    // 2. STFT Analysis
    constexpr std::size_t window_size = 512;
    constexpr std::size_t hop_size    = 128;
    const auto win_type = dsp::WindowType::Hann;

    auto stft_result = dsp::stft(signal, window_size, hop_size, win_type);

    // 3. Extract Magnitude and Phase
    auto mag = dsp::magnitude(stft_result.spectra);
    auto ph  = dsp::phase(stft_result.spectra);

    // -----------------------------------------------------------------------
    // Demonstration 1: Exact Identity Reconstruction (Unmodified)
    // -----------------------------------------------------------------------
    dsp::STFTResult res_identity = stft_result;
    res_identity.spectra = dsp::polar_to_complex(mag, ph);
    auto signal_rec_identity = dsp::istft(res_identity);

    // -----------------------------------------------------------------------
    // Demonstration 2: Frequency Bin Shift
    // -----------------------------------------------------------------------
    const int shift = 20;
    dsp::STFTResult res_shifted = stft_result; // Create separate copy

    for (auto& frame : res_shifted.spectra)
    {
        frame = shift_bins_real(frame, shift);
    }
    auto signal_rec_shifted = dsp::istft(res_shifted);

    // -----------------------------------------------------------------------
    // Demonstration 3: Zero Phase Manipulation ("Magnitude-Only" reconstruction)
    // -----------------------------------------------------------------------
    auto ph_zero = ph;
    for (auto& frame : ph_zero) {
        std::fill(frame.begin(), frame.end(), 0.0);
    }
    
    dsp::STFTResult res_zero_ph = stft_result; // Uses pristine original stft_result
    res_zero_ph.spectra = dsp::polar_to_complex(mag, ph_zero);
    auto signal_rec_zero_ph = dsp::istft(res_zero_ph);

    // -----------------------------------------------------------------------
    // Demonstration 4: Random Phase Noise (Scramble phase)
    // -----------------------------------------------------------------------
    auto ph_noisy = ph;
    std::mt19937 rng(42); // fixed seed for reproducibility
    std::uniform_real_distribution<double> dist(-std::numbers::pi, std::numbers::pi);

    for (auto& frame : ph_noisy) {
        for (auto& val : frame) {
            val = dist(rng); // Replace phase with uniform random noise [-pi, pi]
        }
    }

    dsp::STFTResult res_noisy_ph = stft_result;
    res_noisy_ph.spectra = dsp::polar_to_complex(mag, ph_noisy);
    auto signal_rec_noisy_ph = dsp::istft(res_noisy_ph);

    // -----------------------------------------------------------------------
    // 4. Export Time-Domain Signals for Comparison
    // -----------------------------------------------------------------------
    dsp::saveToCSV("signal_orig.csv", signal, "original");
    dsp::saveToCSV("signal_rec_identity.csv", signal_rec_identity, "identity");
    dsp::saveToCSV("signal_rec_shifted.csv", signal_rec_shifted, "shifted");
    dsp::saveToCSV("signal_rec_zero_ph.csv", signal_rec_zero_ph, "zero_phase");
    dsp::saveToCSV("signal_rec_noisy_ph.csv", signal_rec_noisy_ph, "noisy_phase");

    std::cout << "Reconstruction complete. Files saved to CSV.\n";
    return 0;
}




// int main() 
// {
//     constexpr double fs = 44100.0;
//     constexpr std::size_t N = 1024; // FFT size

//     // 1. Generate a test signal (e.g., sum of two sine waves or a frame of a chirp)
//     auto signal = dsp::generate_chirp(200.0, 4000.0, fs, static_cast<double>(N) / fs, 1.0);
    
//     // Convert signal to complex for FFT
//     std::vector<dsp::Complex> complex_signal(N);
//     for (std::size_t i = 0; i < N; ++i) {
//         complex_signal[i] = dsp::Complex(signal[i], 0.0);
//     }

//     // 2. Compute 1D FFT
//     auto X = dsp::fft(complex_signal);

//     // Truncate to positive frequencies (one-sided spectrum)
//     const std::size_t half_N = N / 2 + 1;
//     std::vector<dsp::Complex> X_half(X.begin(), X.begin() + half_N);

//     // 3. Calculate 1D Magnitude, Wrapped Phase, and Unwrapped Phase
//     auto mag     = dsp::magnitude(X_half);
//     auto phase_w  = dsp::phase(X_half, false); // Wrapped
//     auto phase_uw = dsp::phase(X_half, true);  // Unwrapped

//     // Generate Frequency Bins (Hz) for the x-axis
//     auto freqs = dsp::frequency_bins(N, fs);

//     // 4. Save to a single CSV file with column headers
//     std::vector<std::string> headers = {"frequency_hz", "magnitude", "phase_wrapped", "phase_unwrapped"};
//     dsp::saveColumnsToCSV("spectrum_1d.csv", headers, freqs, mag, phase_w, phase_uw);

//     std::cout << "1D Spectrum data exported to spectrum_1d.csv\n";
//     return 0;
// }


