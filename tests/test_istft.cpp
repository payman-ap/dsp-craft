#include <iostream>
#include <cmath>
#include "stft.hpp"

int main()
{
    constexpr std::size_t window_size = 512;
    constexpr std::size_t hop_size    = 128;
    constexpr double fs               = 44100.0;
    constexpr double duration         = 1.0;

    // 1. Generate & Perform STFT / ISTFT
    auto sig_chirp = dsp::generate_chirp(140.0, 20000.0, fs, duration, 1.0);
    auto stft_chirp = dsp::stft(sig_chirp, window_size, hop_size, dsp::WindowType::Hann);
    auto reconstructed = dsp::istft(stft_chirp);

    // 2. Prepare 2D matrix for CSV (N rows x 3 columns)
    size_t orig_len = sig_chirp.size();
    size_t recon_len = reconstructed.size();
    size_t max_len = std::max(orig_len, recon_len);

    std::vector<std::vector<double>> comparison_matrix(max_len, std::vector<double>(3, 0.0));

    for (size_t i = 0; i < max_len; ++i)
    {
        double orig  = (i < orig_len)  ? sig_chirp[i]     : 0.0;
        double recon = (i < recon_len) ? reconstructed[i] : 0.0;

        comparison_matrix[i][0] = orig;
        comparison_matrix[i][1] = recon;
        comparison_matrix[i][2] = orig - recon; //  error // std::abs(orig - recon)
    }

    // 3. Save comparison CSV
    dsp::saveMatrixToCSV("istft_reconstruction.csv", comparison_matrix);

    return 0;
}



// int main()
// {
//     constexpr std::size_t window_size = 512;
//     constexpr std::size_t hop_size    = 128; // 75% overlap ensures COLA constraint
//     constexpr double fs               = 44100.0;
//     constexpr double duration         = 1.0;

//     // 1. Generate signal
//     auto original_signal = dsp::generate_chirp(140.0, 20000.0, fs, duration, 1.0);

//     // 2. Forward STFT
//     auto stft_result = dsp::stft(original_signal, window_size, hop_size, dsp::WindowType::Hann);

//     // 3. Inverse STFT
//     auto reconstructed_signal = dsp::istft(stft_result);
//     reconstructed_signal.resize(original_signal.size());

//     // 4. Compute Reconstruction Error (RMSE)
//     double mse = 0.0;
//     size_t compare_len = std::min(original_signal.size(), reconstructed_signal.size());

//     for (size_t i = 0; i < compare_len; ++i) {
//         double diff = original_signal[i] - reconstructed_signal[i];
//         mse += diff * diff;
//     }
//     double rmse = std::sqrt(mse / compare_len);

//     std::cout << "Original length: " << original_signal.size() << " samples\n";
//     std::cout << "Reconstructed length: " << reconstructed_signal.size() << " samples\n";
//     std::cout << "Reconstruction RMSE: " << rmse << std::endl;

//     return 0;
// }
