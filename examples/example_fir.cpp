#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "filters/fir.hpp"
#include "signals.hpp"
#include "helper_fcns.hpp"
#include "conv.hpp"


int main()
{
    constexpr double fs = 44100.0; // 44.1 kHz
    constexpr double duration = 0.01; // 10 ms for a quick export check
    constexpr double f1 = 1000.0;
    constexpr double f2 = 10000.0;

    // std::vector<double> h = {0.25, 0.25, 0.25, 0.25};
    // std::vector<double> h(64, 0.015625);

    // auto s1  = dsp::generate_sine(f1, fs, duration, 1.0);
    // auto s2  = dsp::generate_sine(f2, fs, duration, 1.0);

    // std::vector<double> y(s1.size());
    // for (size_t n = 0; n < s1.size(); ++n) {
    //     y[n] = s1[n] + s2[n];
    // }

    std::vector<double> y = {1, 2, 3, 4};
    std::vector<double> h = {2, 3};

    std::vector<double> y_filtered = dsp::fir_filter(y, h);
    std::vector<double> y_conv = dsp::conv(y, h);

    std::cout << "y_filtered = [ ";
    for (double v1 : y_filtered) {
        std::cout << v1 << " ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "y_conv = [ ";
    for (double v2 : y_conv) {
        std::cout << v2 << " ";
    }
    std::cout << "]" << std::endl;

    // Export to CSV for plotting
    std::vector<std::string> headers = {"y", "y_filtered"};
    dsp::saveColumnsToCSV("fir_signals.csv", headers, y, y_filtered);
}






