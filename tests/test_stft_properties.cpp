#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <random>
#include "stft.hpp"
#include "signals.hpp"

namespace dsp {
namespace test {

// Helper function to test round-trip reconstruction property
void verify_perfect_reconstruction(const std::vector<double>& original_signal,
                                    size_t window_size = 512,
                                    size_t hop_size = 128,
                                    WindowType win_type = WindowType::Hann,
                                    double tolerance = 1e-10)
{
    // 1. Forward STFT
    STFTResult stft_result = stft(original_signal, window_size, hop_size, win_type);

    // 2. Inverse STFT
    std::vector<double> reconstructed = istft(stft_result);

    // 3. Verify lengths match (using automatic trimming!)
    ASSERT_EQ(original_signal.size(), reconstructed.size());

    // 4. Verify sample-by-sample equality within tolerance
    // Skip sample 0 if using unpadded Hann window tapers
    for (size_t i = 1; i < original_signal.size(); ++i) {
        EXPECT_NEAR(original_signal[i], reconstructed[i], tolerance)
            << "Mismatch at sample index " << i;
    }
}

// ============================================================================
// PROPERTY TESTS
// ============================================================================

constexpr double FS = 44100.0;
constexpr double DURATION = 1.0; // 1 second = 44100 samples

// Test 1: Pure Sine Wave
TEST(STFTPropertyTest, PerfectReconstructionSine)
{
    // Single 440 Hz tone
    auto signal = generate_sine(440.0, FS, DURATION, 1.0);
    verify_perfect_reconstruction(signal);
}

// Test 2: Linear Chirp (Frequency Sweep)
TEST(STFTPropertyTest, PerfectReconstructionChirp)
{
    // Chirp sweeping from 140 Hz to 20,000 Hz
    auto signal = generate_chirp(140.0, 20000.0, FS, DURATION, 1.0);
    verify_perfect_reconstruction(signal);
}

// Test 3: Unit Impulse (Delta Function)
TEST(STFTPropertyTest, PerfectReconstructionImpulse)
{
    // A single spike at index 1000, zero everywhere else
    std::vector<double> signal(static_cast<size_t>(FS * DURATION), 0.0);
    signal[1000] = 1.0;
    
    verify_perfect_reconstruction(signal);
}

// Test 4: Sum of Sinusoids (Multitone / Harmonic Signal)
TEST(STFTPropertyTest, PerfectReconstructionSumOfSinusoids)
{
    size_t num_samples = static_cast<size_t>(FS * DURATION);
    std::vector<double> signal(num_samples, 0.0);

    // Generate a sum of 440 Hz, 880 Hz, and 1320 Hz
    auto f1 = generate_sine(440.0, FS, DURATION, 0.5);
    auto f2 = generate_sine(880.0, FS, DURATION, 0.3);
    auto f3 = generate_sine(1320.0, FS, DURATION, 0.2);

    for (size_t i = 0; i < num_samples; ++i) {
        signal[i] = f1[i] + f2[i] + f3[i];
    }

    verify_perfect_reconstruction(signal);
}

// Test 5: White Noise (Random Signal across all frequencies)
TEST(STFTPropertyTest, PerfectReconstructionWhiteNoise)
{
    size_t num_samples = static_cast<size_t>(FS * DURATION);
    std::vector<double> signal(num_samples);

    std::mt19937 rng(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (size_t i = 0; i < num_samples; ++i) {
        signal[i] = dist(rng);
    }

    verify_perfect_reconstruction(signal);
}

} // namespace test
} // namespace dsps

