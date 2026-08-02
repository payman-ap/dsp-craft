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

