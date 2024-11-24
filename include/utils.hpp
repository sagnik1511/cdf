#ifndef UTILS_HPP
#define UTILS_HPP

#include <iomanip>
#include <sstream>
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
std::pair<int, _cdfVal> inferAndConvert(const std::string& field) {
    // Try to convert to int
    try {
        size_t pos;
        int intValue = std::stoi(field, &pos);
        if (pos == field.size()) {  // Successful int conversion
            return std::make_pair(0, intValue);
        }
    } catch (...) {
    }

    // Try to convert to double
    try {
        size_t pos;
        double doubleValue = std::stod(field, &pos);
        if (pos == field.size()) {  // Successful double conversion
            return std::make_pair(1, doubleValue);
        }
    } catch (...) {
    }

    // If conversion to int or double failed, treat as a string
    return std::make_pair(2, field);
}

/**
 * @brief Removes trailing characters
 *
 * @param str The corresponding string
 * @param stripChar The character whic is to be removed
 * @returns stripped string from right side
 */
std::string rstrip(const std::string str, const char stripChar) {
    int rIdx = 0;
    for (int i = str.size() - 1; i >= 0; i--) {
        if (str[i] == stripChar) {
            rIdx++;
        } else {
            break;
        }
    }
    return std::string(str.begin(), str.end() - rIdx);
}

/**
 * @brief Prepares double variable to string without truncating precision
 * It is assumed that values will hold 12 digit of precision in general
 *
 * @param str double string value having padding
 * @returns double string with actaul precision
 */
std::string stodst(std::string str) {
    str = rstrip(str, '0');
    if (str[str.size() - 1] == '.') {
        return std::string(str.begin(), str.end() - 1);
    }
    return str;
}

/**
 * @brief Converts variant objects to string
 *
 * @param var a cdf::_cdfVal variant variable
 * @param precision Precision of the double data-type values
 * @return String value of the variant object
 */
std::string toString(const _cdfVal& var, int precision = 12) {
    return std::visit(
        [precision](const auto& value) -> std::string {
            std::ostringstream oss;
            if constexpr (std::is_same_v<std::decay_t<decltype(value)>, double>) {
                oss << std::fixed << std::setprecision(precision) << value;
                return stodst(oss.str());
            } else {
                oss << value;  // Default formatting for non-double types
                return oss.str();
            }
            return oss.str();
        },
        var);
}

/**
 * @brief Converts data-types into string without losing precision for decimal values
 *
 * @param value Actual value
 * @param precision Integer value of precision for fractional values (defaults to 12)
 * @returns Value in string format
 */
template <typename T>
std::string to_string(const T& value, int precision = 12) {
    std::ostringstream oss;
    std::string strAns = "";
    if constexpr (std::is_same_v<T, double>) {
        oss << std::fixed << std::setprecision(precision) << value;
        strAns = stodst(oss.str());
    } else {
        oss << value;
        strAns = oss.str();
    }

    return strAns;
}

#endif
