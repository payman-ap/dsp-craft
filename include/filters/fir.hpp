#ifndef FIR_HPP
#define FIR_HPP

#include <vector>

namespace dsp {

/**
 * @brief Filters input signal x using FIR filter coefficients h.
 * Matches MATLAB's y = filter(h, 1, x).
 * 
 * @param x Input signal vector.
 * @param h FIR impulse response coefficients (filter taps).
 * @return std::vector<double> Output signal with same length as x.
 */
std::vector<double> fir_filter(
    const std::vector<double>& x,
    const std::vector<double>& h);

/**
 * @brief Zero-phase FIR filter (matches MATLAB's y = filtfilt(h, 1, x)).
 * Eliminates phase delay by running the filter forward and backward.
 */
std::vector<double> filtfilt(
    const std::vector<double>& x,
    const std::vector<double>& h);




} // namespace dsp

#endif // FIR_HPP


