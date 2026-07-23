#include <iostream>
#include <vector>
#include <cstddef>
#include <complex>
#include <numbers>
#include <random>
#include <chrono>

using Complex = std::complex<double>;
constexpr double PI = std::numbers::pi;
// constexpr double PI = 3.14159265358979323846;

void printVector(const std::vector<Complex>& vec, const std::string& name = "Vector")
{
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

double magnitudeDifference(
    const std::vector<Complex>& a,
    const std::vector<Complex>& b)
{
    double error = 0.0;

    for(size_t i = 0; i < a.size(); ++i)
    {
        error += std::abs(a[i] - b[i]);
    }

    return error;
}

std::vector<Complex> dft(const std::vector<Complex>& x)
{
    size_t N = x.size();
    std::vector<Complex> X(N);

    // Calculate one frequency bin.
    for (size_t k = 0; k < N; ++k)
    {
        Complex sum = 0.0;

        // Add contribution from every time-domain sample.
        for (size_t n = 0; n < N; ++n)
        {
            Complex W = std::polar(1.0, -2.0 * PI * k * n / N);

            sum += x[n] * W;
        }

        X[k] = sum;
        
    }

    return X;
}

std::vector<Complex> fft(const std::vector<Complex>& x, int depth = 0)
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


int main()
{
    // std::vector<Complex> signal = { 3,1,2,4,5,7,6,8 };
    constexpr std::size_t N = 4096;

    // Random generator: real and imaginary parts in [-1.0, 1.0]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    std::vector<Complex> signal;
    signal.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        double re = dist(gen);
        double im = dist(gen);
        signal.emplace_back(re, im);
    }
    
    auto start_fft = std::chrono::high_resolution_clock::now();
    auto fft_result = fft(signal);
    auto end_fft = std::chrono::high_resolution_clock::now();
    auto duration_fft =
        std::chrono::duration<double, std::micro>
        (end_fft-start_fft)
        .count();

    auto start_dft = std::chrono::high_resolution_clock::now();
    auto dft_result = dft(signal);
    auto end_dft = std::chrono::high_resolution_clock::now();
    auto duration_dft =
        std::chrono::duration<double, std::micro>
        (end_dft-start_dft)
        .count();

    std::cout << "FFT: " << duration_fft << " microseconds\n";
    std::cout << "DFT: " << duration_dft << " microseconds\n";


    // printVector(fft_result, "FFT");
    // printVector(dft_result, "DFT");

    std::cout 
        << "Error = "
        << magnitudeDifference(fft_result,dft_result)
        << "\n";

    return 0;
}
