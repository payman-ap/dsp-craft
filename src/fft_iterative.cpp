#include <iostream>
#include <vector>
#include <complex>
#include <numbers>
#include <cassert>


using Complex = std::complex<double>;
constexpr double PI = std::numbers::pi;

// void printVector(const std::vector<Complex>& vec, const std::string& name = "Vector")
// {
//     std::cout << name << ": [";
//     for (size_t i = 0; i < vec.size(); ++i) {
//         std::cout << vec[i];
//         if (i < vec.size() - 1) std::cout << ", ";
//     }
//     std::cout << "]" << std::endl;
// }


void bit_reverse(std::vector<Complex>& x)
{
    size_t N = x.size();
    // if (N == 0) return {};
    std::vector<Complex> Temp(N); // to be removed and become inplace swap
    size_t n_bits = static_cast<size_t>(std::log2(N));

    std::vector<int> index_bin(n_bits, 0);
    std::vector<int> index_bin_inv(n_bits, 0);

    for (size_t i=0; i < N; i++)
    {
        std::fill(index_bin.begin(), index_bin.end(), 0);
        std::fill(index_bin_inv.begin(), index_bin_inv.end(), 0);
        // convert index to binary
        size_t j = 0; // counter for binary array
        size_t n = i;

        while (n > 0 && j < n_bits)
        {
            index_bin[j] = n % 2;
            n /= 2;
            index_bin_inv[n_bits-1-j] = index_bin[j]; // inverse binary index
            j++;
        }

        std::cout << "bin: ";
        for (int k=static_cast<int>(n_bits)-1; k >= 0; k--)
        {
            std::cout << index_bin[k]; 
        }
        std::cout << std::endl;
        std::cout << "rev: ";
        for (int k=static_cast<int>(n_bits)-1; k >= 0; k--)
        {
            std::cout << index_bin_inv[k]; 
        }

        
        std::cout << "\n" << std::endl;

        // back to integer index
        size_t i_tmp = 0;
        for (size_t k = 0; k < n_bits; k++)
        {
            i_tmp += index_bin_inv[k] * (1ULL << k); //  1ULL:: long long unsigned Promotes the operation to 64-bit unsigned
        }

        // assign the value to the new index
        Temp[i_tmp] = x[i];
    }

    x = Temp;
}


void bit_reverse_adv(std::vector<Complex>& x)
{
    size_t N = x.size();
    size_t j = 0;

    for (size_t i = 0; i < N; ++i)
    {
        if (i < j)
        {
            std::swap(x[i], x[j]);
        }

        // Bitwise increment logic for bit-reversal permutation
        size_t bit = N >> 1;
        while (j & bit)
        {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
    }
}

std::vector<Complex> fft_iterative(const std::vector<Complex>& signal)
{
    size_t N = signal.size();
    assert(N > 0);
    assert((N & (N - 1)) == 0);
    // std::cout << "\n" << "FFT iterative: " << std::endl;
    auto x = signal;
    bit_reverse_adv(x);
    // Combine groups of len 2, 4, 8, ...
    for (size_t len = 2; len <= N; len *= 2)
    {
        #ifdef FFT_DEBUG
            std::cout << "Stage with len = " << len << '\n';
        #endif

        size_t half = len / 2;

        // Compute the fundamental twiddle factor for the current stage length
        Complex Wlen = std::polar(1.0, -2.0 * PI * 1 / len);
        
        for (size_t start = 0; start < N; start += len)
        {
            Complex W = 1.0; // reset W to 1.0 for every block

            #ifdef FFT_DEBUG
                std::cout << "Half: " << '\n';
            #endif

            for (size_t j = 0; j < half; ++j)
            {    
                size_t i1 = start + j;
                size_t i2 = start + j + half;
                // std::cout << i1 << " <-> " << i2 << '\n'; // to be replaced with the butterfly
                // Complex W = std::polar(1.0, -2.0 * PI * j / len);
                Complex u = x[i1];
                Complex v = W * x[i2];
                x[i1] = u + v;
                x[i2] = u - v;

                #ifdef FFT_DEBUG
                    std::cout << "W(" << j << ") = " << W << '\n';
                #endif

                W *= Wlen; // advance twiddle factor for the next iteration
            }

            #ifdef FFT_DEBUG
                std::cout << '\n';
            #endif
        }
        
    }

    return x;

}

// Inverse FFT using the conjugate trick: IFFT(X) = 1/N * conj(FFT(conj(X)))
std::vector<Complex> ifft_iterative(const std::vector<Complex>& X)
{
    size_t N = X.size();
    std::vector<Complex> x(N);

    // 1. Take complex conjugate of input spectrum
    for (size_t i = 0; i < N; ++i) {
        x[i] = std::conj(X[i]);
    }

    // 2. Run standard forward FFT
    x = fft_iterative(x);

    // 3. Take complex conjugate again and scale by 1/N
    for (size_t i = 0; i < N; ++i) {
        x[i] = std::conj(x[i]) / static_cast<double>(N);
    }

    return x;
}


#ifdef FFT_STANDALONE
int main()
{
    // std::vector<Complex> signal = { 3,1,2,4,5,7,6,8 };
    std::vector<Complex> signal = { 0,1,2,3,4,5,6,7 };

    std::cout << "Bit size: " << log2(8) << std::endl;

    auto spectrum = fft_iterative(signal);

    printVector(spectrum, "FFT");

    return 0;
}
#endif

