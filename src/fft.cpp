#include "fft.hpp"
#include <iostream>
#include <cstddef>
#include <complex>
#include <numbers>

namespace dsp {

using Complex = std::complex<double>;
constexpr double PI = std::numbers::pi;
// constexpr double PI = 3.14159265358979323846;

// void printVector(const std::vector<Complex>& vec, const std::string& name)
// {
//     std::cout << name << ": [";
//     for (size_t i = 0; i < vec.size(); ++i) {
//         std::cout << vec[i];
//         if (i < vec.size() - 1) std::cout << ", ";
//     }
//     std::cout << "]" << std::endl;
// }

std::vector<Complex> fft(const std::vector<Complex>& x, int depth)
{
    size_t N = x.size();
    std::vector<Complex> even;
    std::vector<Complex> odd;
    even.reserve(N / 2); // append:reserve but no allocation/construction, will be added with .push_back() 
    odd.reserve(N / 2);
    std::vector<Complex> X(N); // assign: using indexing to allocate the elements

    // std::cout << "fft called with N = " << N << '\n';
    std::string indent(depth * 2, ' '); // constructor of length n, character c
    // std::cout << indent << "fft(N=" << N << ")\n";

    // the base case
    if (N == 1)
    {
        return x;
    }
    
    // split the input in even and odd
    for (size_t i = 0; i < N; i++)
    {
        if (i % 2 == 0)
        {
            even.push_back(x[i]);
        }
        else
        {
            odd.push_back(x[i]);
        }
            
    }

    // printVector(even, "Even");
    // printVector(odd, "Odd");

    auto E = fft(even, depth + 1);
    auto O = fft(odd, depth + 1);

    for (size_t k = 0; k < N / 2; ++k)
    {
        Complex W = std::polar(1.0, -2.0 * PI * k / N);

        Complex t = W * O[k];

        X[k] = E[k] + t;
        X[k + N/2] = E[k] - t;
    }

    return X;
}

std::vector<Complex> ifft(const std::vector<Complex>& X)
{
    const double N = static_cast<double>(X.size());
    std::vector<Complex> temp(X.size());

    for(size_t i=0; i < X.size(); i++)
    {
        temp[i] = std::conj(X[i]);
    }
    auto result = fft(temp);

    for (size_t i = 0; i < result.size(); i++)
    {
        result[i] = std::conj(result[i]) / N;
    }

    return result;
}

} // namespace dsp

#ifdef FFT_STANDALONE
int main()
{
    std::vector<Complex> signal = { 3,1,2,4,5,7,6,8 };
    
    auto spectrum = fft(signal);

    printVector(spectrum, "FFT");

    auto recovered_signal = ifft(spectrum);

    printVector(recovered_signal, "iFFT");

    return 0;
}
#endif

