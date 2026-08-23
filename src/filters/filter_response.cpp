#include "filters/filter_response.hpp"
#include <cmath>
#include <numbers>
#include "helper_fcns.hpp"
#include <iostream>

namespace dsp {



std::vector<Complex> frequency_response(const std::vector<double>& h,
                                        const std::vector<double>& frequencies,
                                        double sample_rate)
{
    std::vector<Complex> H(frequencies.size());
    const double PI = std::numbers::pi;

    for (size_t k = 0; k < frequencies.size(); ++k)
    {
        const double f = frequencies[k];
        const double omega = 2 * PI * f / sample_rate;

        Complex sum(0.0, 0.0);

        for (size_t n = 0; n < h.size(); ++n)
        {
            const double angle = -omega * static_cast<double>(n);
            sum += h[n] * std::polar(1.0, angle);
        }
        H[k] = sum;

    }

    return H;

}




} // namespace dsp






#include "filters/filter_response.hpp"
#include "fft.hpp"
#include "spectrum.hpp"
#include "helper_fcns.hpp"
#include <vector>
#include <cmath>

int main() {
    std::vector<double> h = {0.25, 0.25, 0.25, 0.25}; // 4-tap FIR
    double fs = 44100.0;
    size_t N = 1024; // Zero-padded FFT size

    // 1. Generate Frequency Axis for Positive Bins [0, fs/2]
    // N/2 + 1 bins cover DC to Nyquist
    std::vector<double> freqs = dsp::frequency_bins(N, fs);

    // 2. Continuous DTFT via Direct Summation
    std::vector<dsp::Complex> H_dtft = dsp::frequency_response(h, freqs, fs);
    std::vector<double> mag_dtft_db  = dsp::magnitude_db(H_dtft);

    // 3. Zero-Padded FFT Approach
    // Zero-pad impulse response h to length N
    std::vector<dsp::Complex> h_padded(N, {0.0, 0.0});
    for (size_t i = 0; i < h.size(); ++i) {
        h_padded[i] = {h[i], 0.0};
    }

    // Compute N-point FFT
    std::vector<dsp::Complex> H_fft = dsp::fft(h_padded);

    // Truncate FFT response to positive frequencies [0, N/2]
    H_fft.resize(N / 2 + 1);
    std::vector<double> mag_fft_db = dsp::magnitude_db(H_fft);

    // 4. Export Comparison to CSV
    std::vector<std::string> headers = {"frequency", "mag_dtft_db", "mag_fft_db"};
    dsp::saveColumnsToCSV("filter_comparison.csv", headers, freqs, mag_dtft_db, mag_fft_db);

    return 0;
}


// int main() {
//     // 1. Setup filter and frequency axis
//     std::vector<double> h = {0.25, 0.25, 0.25, 0.25}; // 4-tap Moving Average Filter
//     double fs = 44100.0;
//     size_t num_points = 512;

//     std::vector<double> freqs(num_points);
//     for (size_t i = 0; i < num_points; ++i) {
//         freqs[i] = (static_cast<double>(i) / (num_points - 1)) * (fs / 2.0);
//     }

//     // 2. Compute Complex Frequency Response H(e^(j 2pi f / fs))
//     std::vector<dsp::Complex> H = dsp::frequency_response(h, freqs, fs);

//     // 3. Extract Magnitude and Phase using spectrum.hpp
//     std::vector<double> mag    = dsp::magnitude(H);
//     std::vector<double> mag_db = dsp::magnitude_db(H);
//     std::vector<double> ph     = dsp::phase(H, /*unwrap=*/true);

//     // 4. Extract Real and Imaginary components
//     std::vector<double> real_part(H.size());
//     std::vector<double> imag_part(H.size());
//     for (size_t i = 0; i < H.size(); ++i) {
//         real_part[i] = H[i].real();
//         imag_part[i] = H[i].imag();
//     }

//     // 5. Export all components to CSV using helper_fcns.hpp
//     std::vector<std::string> headers = {
//         "frequency", "magnitude", "magnitude_db", "phase", "real", "imaginary"
//     };

//     dsp::saveColumnsToCSV(
//         "filter_response.csv", 
//         headers, 
//         freqs, mag, mag_db, ph, real_part, imag_part
//     );

//     return 0;
// }