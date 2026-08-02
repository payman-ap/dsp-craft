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
# Core Library Target
# ------------------------------------------------------------------------------
add_library(dsp_craft STATIC
    src/conv.cpp
    src/dft.cpp
    src/fft.cpp
    src/fft_iterative.cpp
    src/spectrum.cpp
    src/windows.cpp
    src/signals.cpp
    src/stft.cpp
)

target_include_directories(dsp_craft PUBLIC include)

# option(FFT_DEBUG "Enable verbose FFT stage/twiddle-factor tracing" OFF)
if(FFT_DEBUG)
    target_compile_definitions(dsp_craft PRIVATE FFT_DEBUG)
endif()

# ------------------------------------------------------------------------------
# Test Executables (tests/)
# ------------------------------------------------------------------------------
add_executable(test_conv tests/test_conv.cpp)
target_link_libraries(test_conv PRIVATE dsp_craft)

add_executable(test_fft tests/test_fft.cpp)
target_link_libraries(test_fft PRIVATE dsp_craft)

add_executable(test_ifft tests/test_ifft.cpp)
target_link_libraries(test_ifft PRIVATE dsp_craft)

add_executable(test_stft tests/test_stft.cpp)
target_link_libraries(test_stft PRIVATE dsp_craft)

add_executable(test_istft tests/test_istft.cpp)
target_link_libraries(test_istft PRIVATE dsp_craft)

# ------------------------------------------------------------------------------
# Example & Benchmark Executables (examples/)
# ------------------------------------------------------------------------------
add_executable(benchmark_conv examples/benchmark_conv.cpp)
target_link_libraries(benchmark_conv PRIVATE dsp_craft)

add_executable(benchmark_fft examples/benchmark_fft.cpp)
target_link_libraries(benchmark_fft PRIVATE dsp_craft)

add_executable(example_stft examples/example_stft.cpp)
target_link_libraries(example_stft PRIVATE dsp_craft)

add_executable(example_window examples/example_window.cpp)
target_link_libraries(example_window PRIVATE dsp_craft)

add_executable(example_signals examples/example_signals.cpp)
target_link_libraries(example_signals PRIVATE dsp_craft)

add_executable(test_spectrum tests/test_spectrum.cpp)
target_link_libraries(test_spectrum PRIVATE dsp_craft)


# ------------------------------------------------------------------------------
# Executables (src/)
# ------------------------------------------------------------------------------

add_executable(run_phase examples/example_phase.cpp)
target_link_libraries(run_phase PRIVATE dsp_craft)

# ------------------------------------------------------------------------------
# GoogleTest Suite
# ------------------------------------------------------------------------------
find_package(GTest REQUIRED)
include(GoogleTest)

# Add your property test executable
add_executable(test_stft_properties tests/test_stft_properties.cpp)

# Link against your library (dsp_craft) and GTest
target_link_libraries(test_stft_properties PRIVATE dsp_craft GTest::gtest GTest::gtest_main)

# Enables 'ctest' discovery
gtest_discover_tests(test_stft_properties)

