#ifndef FILTER_RESPONSE_HPP
#define FILTER_RESPONSE_HPP

#include "spectrum.hpp"
#include <vector>
#include <complex>

namespace dsp {

// using Complex = std::complex<double>;

/**
 * @brief Computes the complex frequency response H(e^(j 2pi f / fs)) of an FIR filter h[n]
 *        at arbitrary frequencies.
 * 
 * @param h Filter impulse response coefficients.
 * @param frequencies Vector of target frequencies in Hz.
 * @param sample_rate Sampling rate in Hz (fs).
 * @return Vector of complex response values H(f) corresponding to each input frequency.
 */
std::vector<Complex> frequency_response(const std::vector<double>& h,
                                        const std::vector<double>& frequencies,
                                        double sample_rate);




} // namespace dsp

#endif // FILTER_RESPONSE_HPP
