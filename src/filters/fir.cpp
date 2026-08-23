#include "filters/fir.hpp"
#include <algorithm>
#include <stdexcept>

namespace dsp {

std::vector<double> fir_filter(const std::vector<double>& x,
                                const std::vector<double>& h)
{
    if (x.empty() || h.empty()) return {};
    
    const size_t N = x.size();
    const size_t M = h.size();

    std::vector<double> y(N, 0.0);

    for (size_t n = 0; n < N; ++n)
    {
        double sum = 0.0;

        // Accumulate terms h[k] * x[n - k]
        // k <= n handles implicit zero-padding for negative indices (x[n-k] = 0 when n < k)
        for (size_t k = 0; k < M && k <=n; ++k)
        {
            sum += h[k] * x[n - k];
        }

        y[n] = sum;

    }

    return y;

}


// Helper: Applies FIR filtering with initial state zi
static std::vector<double> filter_with_zi(
    const std::vector<double>& x, 
    const std::vector<double>& h, 
    const std::vector<double>& zi) 
{
    const size_t N = x.size();
    const size_t M = h.size();
    std::vector<double> y(N, 0.0);
    std::vector<double> state = zi; // Internal filter memory (delay line)

    for (size_t n = 0; n < N; ++n) {
        // Direct Form II Transposed filtering
        y[n] = h[0] * x[n] + (state.empty() ? 0.0 : state[0]);
        
        for (size_t k = 0; k < M - 2; ++k) {
            state[k] = h[k + 1] * x[n] + state[k + 1];
        }
        if (M > 1) {
            state[M - 2] = h[M - 1] * x[n];
        }
    }
    return y;
}


std::vector<double> filtfilt(
    const std::vector<double>& x, 
    const std::vector<double>& h) 
{
    const size_t N = x.size();
    const size_t M = h.size();

    if (N == 0) return {};
    if (M == 1) return x; // Scalar filter trivial case
    if (N < 3 * (M - 1)) {
        throw std::invalid_argument("Input signal x length must be greater than 3 * (h.size() - 1)");
    }

    const size_t nfact = 3 * (M - 1); // MATLAB default padding length

    // Step 1: Compute initial conditions (zi) for steady-state start
    // For FIR filters with a[0] = 1: zi[k] = sum(h[k+1 : M-1])
    std::vector<double> zi(M - 1, 0.0);
    double accum = 0.0;
    for (int k = static_cast<int>(M) - 2; k >= 0; --k) {
        accum += h[k + 1];
        zi[k] = accum;
    }

    // Step 2: Construct padded signal with odd symmetric boundary reflection
    std::vector<double> x_padded;
    x_padded.reserve(N + 2 * nfact);

    // Left boundary padding
    for (size_t i = nfact; i >= 1; --i) {
        size_t idx = i % (N - 1);
        if (idx == 0) idx = N - 1; // Reflect back to start/end points correctly
        x_padded.push_back(2.0 * x[0] - x[idx]);
    }

    // Original signal body
    x_padded.insert(x_padded.end(), x.begin(), x.end());

    // Right boundary padding
    for (size_t i = 0; i < nfact; ++i) {
        size_t idx = (i + 1) % (N - 1);
        x_padded.push_back(2.0 * x[N - 1] - x[N - 1 - idx]);
    }

    // Step 3: Forward Pass with initial state scaling
    std::vector<double> zi_forward(M - 1);
    for (size_t i = 0; i < M - 1; ++i) {
        zi_forward[i] = zi[i] * x_padded[0];
    }
    std::vector<double> y_fwd = filter_with_zi(x_padded, h, zi_forward);

    // Step 4: Reverse the forward result
    std::reverse(y_fwd.begin(), y_fwd.end());

    // Step 5: Backward Pass with initial state scaling
    std::vector<double> zi_backward(M - 1);
    for (size_t i = 0; i < M - 1; ++i) {
        zi_backward[i] = zi[i] * y_fwd[0];
    }
    std::vector<double> y_bwd = filter_with_zi(y_fwd, h, zi_backward);

    // Step 6: Reverse back and extract original region (remove padding)
    std::reverse(y_bwd.begin(), y_bwd.end());

    std::vector<double> y(x.begin(), x.end());
    for (size_t i = 0; i < N; ++i) {
        y[i] = y_bwd[i + nfact];
    }

    return y;
}


} // namespace dsp