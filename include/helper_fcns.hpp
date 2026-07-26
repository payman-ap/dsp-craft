// include/helper_fcns.hpp
#ifndef HELPER_FCNS_HPP
#define HELPER_FCNS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace dsp {

template <typename T>
void printVector(const std::vector<T>& vec, const std::string& name = "Vector")
{
    std::cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Export a single 1D vector to a single-column CSV
template <typename T>
bool saveToCSV(const std::string& filename, const std::vector<T>& data, const std::string& header = "value")
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return false;
    }

    if (!header.empty()) {
        file << header << "\n";
    }

    for (const auto& val : data) {
        file << val << "\n";
    }

    std::cout << "Successfully exported data to " << filename << "\n";
    return true;
}

// Export multiple equal-length vectors as columns to a CSV
template <typename... Args>
bool saveColumnsToCSV(const std::string& filename, const std::vector<std::string>& headers, const Args&... columns)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return false;
    }

    // Write header line
    for (std::size_t i = 0; i < headers.size(); ++i) {
        file << headers[i] << (i + 1 < headers.size() ? "," : "\n");
    }

    // Determine row count from first column
    auto first_col_size = [](const auto& first, const auto&...) { return first.size(); };
    std::size_t num_rows = first_col_size(columns...);

    // Write data rows
    for (std::size_t row = 0; row < num_rows; ++row) {
        std::size_t col_idx = 0;
        auto write_cell = [&](const auto& col) {
            file << col[row];
            col_idx++;
            if (col_idx < sizeof...(columns)) {
                file << ",";
            } else {
                file << "\n";
            }
        };
        (write_cell(columns), ...); // Fold expression over columns
    }

    std::cout << "Successfully exported multi-column data to " << filename << "\n";
    return true;
}

} // namespace dsp

#endif // HELPER_FCNS_HPP




