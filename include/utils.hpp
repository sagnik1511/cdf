#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>

#include "dtypes.hpp"

/**
 * @brief Extracts a subvector from the given vector based on the specified start and end indices.
 *
 * This function ensures that the indices are adjusted if they are out of bounds, and it returns
 * an empty vector if the start index is greater than the end index.
 *
 * @tparam T The data type of the elements in the vector.
 * @param vec The input vector to slice.
 * @param start The starting index (inclusive) of the slice.
 * @param end The ending index (exclusive) of the slice.
 * @return A subvector containing elements from the start to end indices.
 */
template <typename T>
std::vector<T> slice(const std::vector<T>& vec, int start, int end) {
    if (start < 0)
        start = 0;
    if (end > vec.size())
        end = vec.size();
    if (start > end)
        return {};

    return std::vector<T>(vec.begin() + start, vec.begin() + end);
}

/**
 * @brief Infers and converts a string field to the most appropriate `_cdfVal` type.
 *
 * This function attempts to interpret the input string as an integer, double, or string,
 * in that order of precedence. If the conversion to integer or double fails, the input
 * is returned as a string.
 *
 * @param field The input string to infer and convert.
 * @return The inferred and converted `_cdfVal` type, which can be an integer, double, or string.
 */
_cdfVal inferAndConvert(const std::string& field) {
    // Try to convert to int
    try {
        size_t pos;
        int intValue = std::stoi(field, &pos);
        if (pos == field.size()) {  // Successful int conversion
            return intValue;
        }
    } catch (...) {
    }

    // Try to convert to double
    try {
        size_t pos;
        double doubleValue = std::stod(field, &pos);
        if (pos == field.size()) {  // Successful double conversion
            return doubleValue;
        }
    } catch (...) {
    }

    // If conversion to int or double failed, treat as a string
    return field;
}

#endif
