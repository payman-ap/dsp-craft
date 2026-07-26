#include <iostream>
#include <vector>


enum class WindowType
{
    Rectangular,
    Hann,
    Hamming,
    Blackman
};

inline std::vector<double> createWindow(size_t N, WindowType type)
{
    //
}