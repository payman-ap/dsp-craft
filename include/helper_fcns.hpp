// include/helper_fcns.hpp
#ifndef HELPER_FCNS_HPP
#define HELPER_FCNS_HPP

#include <iostream>
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

} // namespace dsp

#endif // HELPER_FCNS_HPP




