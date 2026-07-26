#include "stft.hpp"


namespace dsp {

std::vector<double> extract_frame( const std::vector<double>& signal, size_t start, size_t window_size )
{
    std::vector<double> frame(window_size, 0.0);
    // size_t idx_start = 0;

    for (size_t i = 0; i < window_size; ++i)
    {
        // Safe bound checking: pad with 0.0 if reaching past end of signal
        if (start + i < signal.size()) {
            frame[i] = signal[start + i];
        }
    }

    return frame;
}

void apply_window(std::vector<double>& frame, const std::vector<double>& window)
{
    assert(frame.size() == window.size());
    size_t N = frame.size();

    for (size_t i = 0; i < N; ++i)
    {
        frame[i] *= window[i];
    }
}

std::vector<std::vector<Complex>> stft( const std::vector<double>& signal, size_t window_size, size_t hop_size, WindowType type)
{
    std::vector<std::vector<Complex>> stft_matrix;

    // Safety checks
    if (signal.empty() || window_size == 0 || hop_size == 0 || signal.size() < window_size)
    {
        return stft_matrix;
    }

    // 1. Generate window coefficients
    std::vector<double> win = create_window(window_size, type, true);

    // 2. Calculate the number of total frames
    size_t num_frames = (signal.size() - window_size) / hop_size + 1;
    stft_matrix.reserve(num_frames);

    // 3. Iterate over the signal with step size `hop_size`
    // for (size_t start = 0; start + window_size <= signal.size(); start += hop_size)
    for (size_t start = 0; start < signal.size(); start += hop_size) // with zero padding
    {
        // Extract and window the current frame (real)
        auto frame = extract_frame(signal, start, window_size);
        apply_window(frame, win);

        // Convert double samples to Complex type for FFT input
        std::vector<Complex> X(window_size);
        for (size_t i = 0; i < window_size; ++i)
        {
            X[i] = Complex(frame[i], 0.0);
        }

        // FFT implementation (in-place or returning vector)
        X = fft_iterative(X);

        // Append the frame spectrum to the matrix
        stft_matrix.push_back(std::move(X));


    }

    return stft_matrix;


}



} // namespace dsp


int main ()
{
    constexpr std::size_t window_size = 512;
    constexpr std::size_t hop_size    = 128;

    constexpr double fs = 44100.0;
    constexpr double duration = 1.0; // 1000 ms ensures signal length > window_size
    auto sig_chirp = dsp::generate_chirp(140.0, 20000.0, fs, duration, 1.0);
    
    // --- Test 1: Simple Signal ---
    std::vector<double> signal = {0,1,2,3,4,5,6,7};
    size_t small_win_size = 4;
    auto stft_matrix = dsp::stft(signal, small_win_size, 2, dsp::WindowType::Rectangular);
    auto stft_mag = dsp::magnitude(stft_matrix);
    dsp::saveMatrixToCSV("stft_matrix.csv", stft_mag);

    // --- Test 2: Chirp Signal ---
    auto stft_chirp = dsp::stft(sig_chirp, window_size, hop_size, dsp::WindowType::Hann);
    auto stft_mag_chirp = dsp::magnitude_db(stft_chirp);
    dsp::saveMatrixToCSV("stft_chirp.csv", stft_mag_chirp);
    auto spec = dsp::spectrogram(stft_chirp);
    dsp::saveMatrixToCSV("spec.csv", spec);



    return 0;
}