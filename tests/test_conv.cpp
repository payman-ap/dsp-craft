// examples/test_conv.cpp
#include "conv.hpp"
#include "helper_fcns.hpp"
#include <vector>

int main()
{
    std::vector<double> x = {1, 2, 3};
    std::vector<double> h = {1, -1};

    auto y = dsp::conv(x, h);
    dsp::printVector(y, "CONV");

    // Case 1: Default N (N = 3 + 2 - 1 = 4). Automatically zero-pads!
    // Gives exact linear convolution output.
    auto yc_default = dsp::cconv(x, h);
    dsp::printVector(yc_default, "CCONV (Default)"); 

    // Case 2: Explicit N = 3 (Unpadded circular convolution)
    // Causes time-domain aliasing (wrapping)
    auto yc_3 = dsp::cconv(x, h, 3);
    dsp::printVector(yc_3, "CCONV (N=3, Aliased)");

    // Case 3: Explicit N = 6 (Extra zero-padding)
    auto yc_6 = dsp::cconv(x, h, 6);
    dsp::printVector(yc_6, "CCONV (N=6, Extra Padded)");

    auto yfft = dsp::fftconv(x, h);
    dsp::printVector(yfft, "FFT CONV");

    return 0;
}