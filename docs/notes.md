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
Real DFT
        ↓
Benchmark DFT vs FFT
        ↓
Iterative FFT
        ↓
Inverse FFT
        ↓
FFT-based convolution
        ↓
Audio spectrum analyzer
        ↓
STFT
        ↓
Real-time DSP projects
```

