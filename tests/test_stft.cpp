#include <iostream>
#include <cmath>
#include "stft.hpp"

int main ()
{
    constexpr std::size_t window_size = 512;
    constexpr std::size_t hop_size    = 128;
    

    // --- Test 1: Simple Signal ---
    std::vector<double> signal = {0,1,2,3,4,5,6,7};
    size_t small_win_size = 4;
    auto stft_result = dsp::stft(signal, small_win_size, 2, dsp::WindowType::Rectangular);
    auto stft_mag = dsp::magnitude(stft_result.spectra);
    dsp::saveMatrixToCSV("stft_matrix.csv", stft_mag);

    // --- Test 2: Chirp Signal ---
    constexpr double fs = 44100.0;
    constexpr double duration = 1.0; // 1000 ms ensures signal length > window_size
    auto sig_chirp = dsp::generate_chirp(140.0, 20000.0, fs, duration, 1.0);

    auto stft_chirp = dsp::stft(sig_chirp, window_size, hop_size, dsp::WindowType::Hann);
    auto stft_mag_chirp = dsp::magnitude_db(stft_chirp.spectra);
    dsp::saveMatrixToCSV("stft_chirp.csv", stft_mag_chirp);
    auto spec = dsp::spectrogram(stft_chirp.spectra);
    dsp::saveMatrixToCSV("spec.csv", spec);



    return 0;
}
