#include "signals.hpp"
#include "helper_fcns.hpp"
#include <iostream>
#include <vector>

int main()
{
    constexpr double fs = 44100.0; // 44.1 kHz
    constexpr double duration = 0.01; // 10 ms for a quick export check

    auto sine  = dsp::generate_sine(440.0, fs, duration, 1.0);
    auto chirp = dsp::generate_chirp(100.0, 1000.0, fs, duration, 1.0);
    auto sq    = dsp::generate_square(440.0, fs, duration, 1.0, 0.5);
    auto noise = dsp::generate_noise(fs, duration, 0.5);

    std::cout << "Generated signals (" << sine.size() << " samples each):\n";
    std::cout << "Sine[0]:   " << sine[0] << "\n";
    std::cout << "Chirp[0]:  " << chirp[0] << "\n";
    std::cout << "Square[0]: " << sq[0] << "\n";

    // Export to CSV for plotting
    std::vector<std::string> headers = {"sine", "chirp", "square", "noise"};
    dsp::saveColumnsToCSV("generated_signals.csv", headers, sine, chirp, sq, noise);

    return 0;
}

