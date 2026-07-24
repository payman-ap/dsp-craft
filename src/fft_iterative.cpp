#include <iostream>
#include <vector>
#include <complex>


using Complex = std::complex<double>;

void printVector(const std::vector<Complex>& vec, const std::string& name = "Vector")
{
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}



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


int main()
{
    // std::vector<Complex> signal = { 3,1,2,4,5,7,6,8 };
    std::vector<Complex> signal = { 0,1,2,3,4,5,6,7 };
    
    bit_reverse(signal);

    printVector(signal, "REV");

    std::cout << "Bit size: " << log2(8) << std::endl;

    return 0;
}

