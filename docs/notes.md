# Notes

## Build

```
cmake -B build                   # Run once to set up
cmake --build build -j           # Compile all targets using parallel jobs
./build/run_fft                  # Run directly from root
```

or  older style:

```
mkdir build && cd build
cmake ..
make -j
./run_dft
```

## Road map

```
✓ Recursive FFT
        ↓
✓ Real DFT
        ↓
✓ Benchmark DFT vs FFT
        ↓
✓ Iterative FFT
        ↓
✓ Inverse FFT
        ↓
✓ FFT-based convolution
        ↓
✓ Window functions
        ↓
✓ STFT
        ↓
✓ Spectrogram
        ↓
✓ IFFT
        ↓
✓ Overlap-Add
        ↓
✓ ISTFT
        ↓
✓ Perfect reconstruction tests
        ↓
Mel spectrogram
        ↓
Phase vocoder
        ↓
Pitch shifting
        ↓
Time stretching
        ↓
Real-time audio spectrum analyzer
        ↓
Mini DSP library
        ↓
Real-time DSP projects
```

### Convolution roadmap:

```
✓ Time-domain convolution
        ↓
✓ Naive C++ implementation
        ↓
✓ Verify with small examples
        ↓
✓ Understand circular convolution
        ↓
✓ Zero-padding (FFT Friendly?! power of 2)
        ↓
✓ FFT convolution
        ↓
✓ Compare outputs
        ↓
✓ Benchmark O(N²) vs O(N log N)
```

## Advanced Roadmap

```
✓ FFT
        ↓
✓ IFFT
        ↓
✓ FFT convolution
        ↓
✓ Window functions
        ↓
✓ STFT
        ↓
✓ Spectrogram
        ↓
Mel Spectrogram
        ↓
Overlap-Add reconstruction
        ↓
ISTFT
        ↓
Phase vocoder
        ↓
Real-time audio analyzer
        ↓
Mini DSP library
```

### Ongoing

```
✓ Window Functions
        ↓
✓ Window tests
        ↓
✓ CSV export helper
        ↓
✓ Synthetic signal generator
        ↓
✓ STFT
        ↓
✓ Magnitude
        ↓
✓ Spectrogram
        ↓
✓ Image
```



## Next ideas

STFT, filters, spectrum analyzers, real-time DSP


FFT
│
├── ✓ Convolution
├── ✓ STFT
├── ✓ Spectrogram
├── Overlap-add
├── ISTFT
├── Mel filter bank
├── MFCC
├── Phase vocoder
├── Pitch shifting
└── Time stretching



The next features—Mel spectrograms, phase vocoder, time stretching

GTest


## To do:

Magnitude/phase helper functions (small, reusable utilities)
Spectrogram export (CSV or PGM image) to verify your STFT visually
STFT utility class (clean accessors and decomposition helpers)
Phase vocoder core (phase propagation only)
Time stretching
Pitch shifting
Mel filter bank
Mel spectrogram


I'd wait until you start the phase vocoder. The temporal phase unwrapping depends on:

hop size
FFT size
expected phase advance
sample rate

Those parameters belong to the phase-vocoder algorithm




in STFTResult

```cpp
std::vector<std::vector<Complex>> spectra;
```

becomes


```cpp
size_t num_frames() const;
size_t fft_size() const;

const std::vector<Complex>& frame(size_t i) const;
std::vector<Complex>& frame(size_t i);
```

phase vocoder

```cpp
for (size_t t = 1; t < stft.num_frames(); ++t)
{
    auto& current = stft.frame(t);
    auto& previous = stft.frame(t - 1);

    ...
}

```



## Filter

Numerical Implementation in C++

When we transition to coding, we'll cover topics that don't appear in many DSP books:

Direct convolution vs. circular convolution
Cache-friendly coefficient layouts
Circular buffers (avoid shifting samples every iteration)
Block processing
SIMD vectorization
FFT-based FIR for long filters
Floating-point accuracy
Fixed-point considerations (if you're interested later)

| Topic                     | Importance |
| ------------------------- | ---------: |
| Linear phase              |      ⭐⭐⭐⭐⭐ |
| Minimum phase             |      ⭐⭐⭐⭐⭐ |
| Filter specifications     |      ⭐⭐⭐⭐⭐ |
| Window method             |      ⭐⭐⭐⭐⭐ |
| Parks-McClellan intuition |      ⭐⭐⭐⭐☆ |
| Group delay               |      ⭐⭐⭐⭐☆ |
| Numerical implementation  |      ⭐⭐⭐⭐⭐ |
| Polyphase & multirate     |      ⭐⭐⭐⭐☆ |
| FIR structures            |      ⭐⭐⭐☆☆ |


Where I'd go next

At this point, I think you've reached a solid conceptual understanding of FIR design.

The next topic I'd choose is implementation architecture, not more theory.

Specifically:

How would you design a reusable FIR filter library in modern C++?

That opens up interesting engineering questions:

FIRFilter<T> class design,
circular buffers versus shifting samples,
compile-time (std::array) versus runtime (std::vector) tap counts,
streaming APIs versus block-processing APIs,
coefficient storage, alignment, and SIMD-friendly layouts,
and how libraries like Eigen, Intel IPP, CMSIS-DSP, or Liquid-DSP organize these ideas.



### Filter roadmap

```
                FILTERS
                   │
                   ▼
        ┌─────────────────────┐
        │  1. FIR foundation  │
        └─────────────────────┘
                   │
                   ▼
        Direct-form FIR
                   │
                   ▼
        FIR frequency response
                   │
                   ▼
        Moving-average filter
                   │
                   ▼
        Windowed-sinc LPF
                   │
                   ▼
        HPF / BPF / BSF
                   │
                   ▼
        Window comparison
                   │
                   ▼
        ┌─────────────────────┐
        │  2. FIR analysis    │
        └─────────────────────┘
                   │
          impulse response
          step response
          frequency response
          phase / group delay
          linear phase
                   │
                   ▼
        ┌─────────────────────┐
        │  3. IIR foundation  │
        └─────────────────────┘
                   │
                   ▼
        Difference equations
                   │
                   ▼
        Direct Form I
                   │
                   ▼
        Direct Form II
                   │
                   ▼
        First/second-order IIR
                   │
                   ▼
        Biquad
                   │
                   ▼
        ┌─────────────────────┐
        │  4. IIR design      │
        └─────────────────────┘
                   │
                   ▼
        Butterworth
                   │
                   ▼
        Chebyshev
                   │
                   ▼
        Pole / zero analysis
                   │
                   ▼
        Stability experiments
                   │
                   ▼
        ┌─────────────────────┐
        │  5. Filter toolbox  │
        └─────────────────────┘
                   │
                   ▼
        FIR + IIR common API
                   │
                   ▼
        Filter frequency response
                   │
                   ▼
        Cascading filters
                   │
                   ▼
        Streaming processing
                   │
                   ▼
        Real-time filtering
                   │
                   ▼
        ┌─────────────────────┐
        │  6. FFT filtering   │
        └─────────────────────┘
                   │
                   ▼
        Overlap-save
                   │
                   ▼
        Partitioned convolution
                   │
                   ▼
        Real-time convolution
                   │
                   ▼
             Mini DSP Library
                   │
                   ▼
           Real-time DSP
```


```
✓ FIR direct-form filtering
        ↓
✓ FIR unit tests
        ↓
✓ impulse response
        ↓
✓ step response
        ↓
✓ FIR frequency response
        ↓
✓ moving-average filter
        ↓
✓ efficient moving-average implementation
        ↓
✓ benchmark naive vs efficient
        ↓
✓ windowed-sinc LPF
        ↓
✓ LPF with different windows
        ↓
✓ LPF frequency-response analysis
```
