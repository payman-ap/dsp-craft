cmake_minimum_required(VERSION 3.20)
project(algo_craft LANGUAGES CXX)

# Enforce modern C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Debug build if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Global target include directory setup macro or base function
# (We will apply includes directly to targets below)

# ------------------------------------------------------------------------------
# Compiler Warning & Debug Flags
# ------------------------------------------------------------------------------
if(MSVC)
    add_compile_options(/W4 /Zi /Od)
else()
    # GCC / Clang:
    # -g      : Output debug symbols
    # -O0     : Turn off optimization for accurate line-by-line debugging
    # -Wall   : Enable standard warnings
    # -Wextra : Enable extra detailed warnings
    # -Wpedantic : Strictly follow C++ standard rules
    add_compile_options(-g -O0 -Wall -Wextra -Wpedantic)
endif()

# ------------------------------------------------------------------------------
# Executable 1: Discrete Fourier Transform (DFT)
# ------------------------------------------------------------------------------
add_executable(run_dft
    src/dft.cpp
)
target_include_directories(run_dft PRIVATE include)

# ------------------------------------------------------------------------------
# Executable 2: Fast Fourier Transform (FFT)
# ------------------------------------------------------------------------------
add_executable(run_fft
    src/fft.cpp
)
target_include_directories(run_fft PRIVATE include)

# ------------------------------------------------------------------------------
# Executable 3: Bit Reverse
# ------------------------------------------------------------------------------
add_executable(run_bitrev
    src/fft_iterative.cpp
)
target_include_directories(run_bitrev PRIVATE include)


