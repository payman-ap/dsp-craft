#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "filters/fir.hpp"
#include "conv.hpp"

int main() {
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> h = {2.0, 3.0};

    std::vector<double> y = dsp::fir_filter(x, h);

    // Expected MATLAB filter(h, 1, x) output: [10.0, 40.0, 70.0]
    std::vector<double> expected = {2.0, 7.0, 12.0, 17.0};

    std::vector<double> y2 = dsp::conv(x, h);

    std::cout << "y = [ ";
    for (double v : y) {
        std::cout << v << " ";
    }
    std::cout << "]" << std::endl;


    std::cout << "y2 = [ ";
    for (double v2 : y2) {
        std::cout << v2 << " ";
    }
    std::cout << "]" << std::endl;


    return 0;

}

// int main ()
// {
    // std::vector<double> x = {1.0, 2.0, 3.0, 4.0};
    // std::vector<double> h = {2.0, 3.0};

    // std::vector<double> y = dsp::fir_filter(x, h);

    // // Expected MATLAB filter(h, 1, x) output: [10.0, 40.0, 70.0]
    // std::vector<double> expected = {2.0, 7.0, 12.0, 17.0};

    // assert(y.size() == expected.size());
    // for (size_t i = 0; i < y.size(); ++i) {
    //     assert(std::abs(y[i] - expected[i]) < 1e-9);
    // }

    // std::cout << "FIR filter test passed!" << std::endl;

    // // Output should match MATLAB's filtfilt(h, 1, x) = [900, 1800, 2700]
    // // std::vector<double> x1 = {1.0, 2.0, 3.0, 4.0};
    // std::vector<double> y1 = dsp::filtfilt(x, h);

    // std::cout << "filtfilt output: ";
    // for (double val : y1) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;

    // std::vector<double> expected1 = {900.0, 1800.0, 2700.0};
    // for (size_t i = 0; i < y1.size(); ++i) {
    //     assert(std::abs(y1[i] - expected1[i]) < 1e-6);
    // }

    // std::cout << "filtfilt test passed!" << std::endl;


    // // Custom

    // std::vector<double> x2 = {1.0, 0.0, 0.0, 0.0, 0.0};
    // std::vector<double> h2 = {0.2, 0.4, 0.3, 0.1};

    // std::vector<double> y2 = dsp::fir_filter(x2, h2);

    // std::cout << "y2 = [ ";
    // for (double v : y2) {
    //     std::cout << v << " ";
    // }
    // std::cout << "]" << std::endl;


    // return 0;
// }
