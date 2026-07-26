#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <cstddef>
#include <vector>

namespace dsp {

// Generates a sine wave: A * sin(2 * pi * f * t + phase)
std::vector<double> generate_sine(double freq,
                                  double fs,
                                  double duration,
                                  double amplitude = 1.0,
                                  double phase_rad = 0.0);

// Generates a linear frequency-swept chirp from f0 to f1 over the given duration
std::vector<double> generate_chirp(double f0,
                                   double f1,
                                   double fs,
                                   double duration,
                                   double amplitude = 1.0);

// Generates a square wave with a specified duty cycle (0.0 to 1.0)
std::vector<double> generate_square(double freq,
                                    double fs,
                                    double duration,
                                    double amplitude = 1.0,
                                    double duty_cycle = 0.5);

// Generates uniform white noise in [-amplitude, amplitude]
std::vector<double> generate_noise(double fs,
                                   double duration,
                                   double amplitude = 1.0);

// Generates zero-mean Gaussian (normal) white noise with given std_dev
std::vector<double> generate_gaussian_noise(double fs,
                                            double duration,
                                            double std_dev = 1.0);

} // namespace dsp

#endif // SIGNALS_HPP


