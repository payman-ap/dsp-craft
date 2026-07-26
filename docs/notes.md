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
Audio spectrum analyzer
        ↓
STFT
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
STFT
        ↓
✓ Magnitude
        ↓
Spectrogram
        ↓
Image
```



## Next ideas

STFT, filters, spectrum analyzers, real-time DSP


FFT
│
├── Convolution
├── STFT
├── Spectrogram
├── Overlap-add
├── ISTFT
├── Mel filter bank
├── MFCC
├── Phase vocoder
├── Pitch shifting
└── Time stretching








