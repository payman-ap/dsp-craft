#include "spectrum.hpp"
#include "fft.hpp"
#include "signals.hpp"
#include "helper_fcns.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

int main()
{
    std::cout << "=== Testing Spectrum Functions ===\n\n";

    // 1. Generate a test signal (sine wave) and run FFT
    constexpr double fs = 1000.0;
    constexpr double duration = 0.1; // 100 samples
    auto signal = dsp::generate_sine(50.0, fs, duration, 2.0); // 50 Hz sine, amplitude 2.0
    
    // Convert double vector to complex vector for FFT
    std::vector<dsp::Complex> complex_sig(signal.begin(), signal.end());
    auto X = dsp::fft(complex_sig);

    // 2. Test Magnitude and Magnitude dB
    auto mag = dsp::magnitude(X);
    auto mag_db = dsp::magnitude_db(X);

    std::cout << "Signal length: " << signal.size() << " samples\n";
    std::cout << "Peak Magnitude: " << mag[5] << "\n";
    std::cout << "Peak Magnitude (dB): " << mag_db[5] << " dB\n\n";

    // 3. Test Phase Wrapping vs Unwrapping
    // Construct a synthetic phase response with intentional 2*pi wraps
    std::vector<dsp::Complex> synthetic_X(100);
    for (size_t i = 0; i < 100; ++i) {
        double linear_phase = -0.15 * static_cast<double>(i); // Accumulates phase beyond -pi
        synthetic_X[i] = std::polar(1.0, linear_phase);
    }

    auto wrapped_ph = dsp::phase(synthetic_X, false);
    auto unwrapped_ph = dsp::phase(synthetic_X, true);

    std::cout << "Sample 50 Phase (Wrapped):   " << wrapped_ph[50] << " rad\n";
    std::cout << "Sample 50 Phase (Unwrapped): " << unwrapped_ph[50] << " rad\n\n";

    // Verify unwrap continuity: consecutive differences in unwrapped phase should be ~ -0.15
    double diff = unwrapped_ph[50] - unwrapped_ph[49];
    std::cout << "Unwrapped consecutive diff at sample 50: " << diff << " (Expected ~ -0.15)\n";

    // 4. Test Polar to Complex Reconstruction
    auto reconstructed = dsp::polar_to_complex(mag, wrapped_ph);
    
    double max_err = 0.0;
    for (size_t i = 0; i < X.size(); ++i) {
        max_err = std::max(max_err, std::abs(X[i] - reconstructed[i]));
    }
    std::cout << "Reconstruction Error (Original vs Reconstructed Spectrum): " << max_err << "\n";

    if (max_err < 1e-10) {
        std::cout << "\n>>> ALL SPECTRUM TESTS PASSED SUCCESSFULY! <<<\n";
    } else {
        std::cout << "\n>>> TEST FAILED: Reconstruction error too high! <<<\n";
    }

    return 0;
}

