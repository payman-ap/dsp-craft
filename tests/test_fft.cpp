// tests/test_fft.cpp
#include "fft_iterative.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

bool is_close(dsp::Complex a, dsp::Complex b, double tol = 1e-6) {
    return std::abs(a - b) < tol;
}

int main() {
    std::vector<dsp::Complex> input = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
    
    // Test Forward FFT
    auto spectrum = dsp::fft_iterative(input);
    
    // Test Inverse FFT (Reconstruction)
    auto reconstructed = dsp::ifft_iterative(spectrum);
    
    for (size_t i = 0; i < input.size(); ++i) {
        assert(is_close(input[i], reconstructed[i]));
    }

    std::cout << "[PASS] Iterative FFT/IFFT reconstruction test passed!\n";
    return 0;
}
