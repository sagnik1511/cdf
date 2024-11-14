#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>


// Slice a vector based upon first and last index
// Fixes the ranges if indexes are out of range
template <typename T>
std::vector<T> slice(const std::vector<T>& vec, int start, int end) {
    if (start < 0) start = 0;
    if (end > vec.size()) end = vec.size();
    if (start > end) return {};

    return std::vector<T>(vec.begin() + start, vec.begin() + end);
}


#endif