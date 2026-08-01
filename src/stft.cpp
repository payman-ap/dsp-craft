#include "stft.hpp"
#include <cmath>

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

STFTResult stft( const std::vector<double>& signal, size_t window_size, size_t hop_size, WindowType type)
{
    STFTResult result;
    result.config.fft_size = window_size;
    result.config.hop_size = hop_size;
    result.config.window   = type;

    // Safety checks
    if (signal.empty() || window_size == 0 || hop_size == 0)
    {
        return result;
    }

    // 1. Generate window coefficients
    std::vector<double> win = create_window(window_size, type, true);
    // store as meta data
    result.original_signal_length = signal.size();
    result.analysis_window = win;

    // 2. Calculate the number of total frames
    size_t num_frames = (signal.size() + hop_size - 1) / hop_size;
    result.spectra.reserve(num_frames);

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
        result.spectra.push_back(std::move(X));


    }

    return result;


}


// ISTFT
std::vector<double> istft(const STFTResult& stft_result)
{
    const auto& spectra = stft_result.spectra;
    const size_t window_size = stft_result.config.fft_size;
    const size_t hop_size    = stft_result.config.hop_size;
    // const WindowType win_type = stft_result.config.window;
     

    if (spectra.empty() || window_size == 0 || hop_size == 0) {
        return {};
    }

    const size_t num_frames = spectra.size();

    // 1. Calculate total output signal length
    // Max index written to is: (num_frames - 1) * hop_size + window_size
    const size_t output_length = (num_frames - 1) * hop_size + window_size;

    std::vector<double> reconstructed(output_length, 0.0);
    std::vector<double> window_sum(output_length, 0.0); // For normalization

    // 2. Re-create the synthesis window
    std::vector<double> win = stft_result.analysis_window; // create_window(window_size, win_type, true);

    // 3. Process each frame (Inverse FFT -> Window -> Overlap-Add)
    for (size_t k = 0; k < num_frames; ++k)
    {
        size_t start = k * hop_size;

        // Perform Inverse FFT
        // Assumes ifft_iterative returns vector<Complex> scaled by 1/N
        std::vector<Complex> time_frame_complex = ifft_iterative(spectra[k]);

        for (size_t i = 0; i < window_size; ++i)
        {
            size_t signal_idx = start + i;

            if (signal_idx < output_length)
            {
                // Take the real part of the inverse FFT
                double real_val = time_frame_complex[i].real();

                // Apply synthesis window and accumulate into output buffer
                reconstructed[signal_idx] += real_val * win[i];

                // Accumulate squared window values for amplitude normalization
                window_sum[signal_idx] += win[i] * win[i];
            }
        }
    }

    // 4. Normalize by window overlap sum
    // (Avoid division by zero near signal edges or empty frames)
    constexpr double eps = 1e-12;
    for (size_t i = 0; i < output_length; ++i)
    {
        if (window_sum[i] > eps) {
            reconstructed[i] /= window_sum[i];
        }
    }

    // Trim to original length
    if (stft_result.original_signal_length > 0 && 
        stft_result.original_signal_length <= output_length) 
    {
        reconstructed.resize(stft_result.original_signal_length);
    }

    return reconstructed;
}



} // namespace dsp



