# algo-craft

A lightweight, zero-dependency C++20 library and playground for implementing fundamental numerical, digital signal processing (DSP), and algorithmic primitives from scratch.

---

## 🎯 Purpose

The goal of **algo-craft** is to unpack complex mathematical and signal processing algorithms by building them from first principles using modern C++20. By avoiding heavy external dependencies, this project focuses on clear logic, mathematical correctness, and understanding low-level performance characteristics.

---

## 🚀 Features & Modules

* **DSP Core:** Discrete Fourier Transform (DFT), Fast Fourier Transform (FFT), windowing, and spectral utilities.
* **Modern C++20 Standard:** Leverages standard library numerical features (`<numbers>`, `<complex>`, `<concepts>`).
* **Modular Executables:** Independent entry points for prototyping, testing, and debugging individual algorithms.
* **Zero External Dependencies:** Only requires a C++20 compliant compiler and CMake.

---

## 📁 Project Structure

```text
algo-craft/
├── include/
│   └── algo_craft/     # Header files & algorithm interfaces
├── src/                # Source files & executable entry points
│   ├── dft.cpp
│   └── fft.cpp
├── docs/               # Architecture notes & mathematical derivations
│   └── notes.md
└── CMakeLists.txt      # Build configuration
```
## 🛠️ Building & Running

Prerequisites

* Compiler: GCC 10+, Clang 11+, or MSVC with C++20 support
* Build System: CMake 3.20+

Quick Start

1. Configure the project:

```bash
cmake -B build
```

2. Compile all targets:


```bash
cmake --build build -j
```

3. Run an algorithm target:

```bash
# Run the Discrete Fourier Transform demo
./build/run_dft

# Run the Fast Fourier Transform demo
./build/run_fft
```





