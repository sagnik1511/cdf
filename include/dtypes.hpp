#ifndef DTYPES_HPP
#define DTYPES_HPP

#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace cdf {

/**
 * @brief Data types with their rank in order of precedence.
 *
 * This vector represents the order of precedence for the supported data types.
 * The order is defined as follows:
 * - `"int"`
 * - `"double"`
 * - `"string"`
 */
std::vector<std::string> dTypeWithRank = {"int", "double", "string"};

/**
 * @brief A class to represent a "Not a Number" (NaN) value.
 *
 * The `NaN` class is used to signify missing or undefined values. It provides
 * support for arithmetic operations and comparisons. Instances of the `NaN`
 * class can be used seamlessly within computations and comparisons where
 * missing values need to be handled gracefully.
 *
 * Example:
 * ```
 * cdf::NaN nan = cdf::NaN();
 * ```
 *
 * - The stream insertion operator (`<<`) is overloaded to output an empty string for `NaN`.
 * - All comparison operators are overloaded to reflect that:
 *   - `NaN` is not equal to any value (except itself).
 *   - `NaN` is always not comparable (e.g., `<`, `>`) with other values.
 */
class NaN {
   public:
    /**
     * @brief Default constructor for the NaN class.
     */
    NaN() = default;

    /**
     * @brief Overloaded stream insertion operator.
     *
     * Outputs an empty string when a `NaN` instance is streamed.
     *
     * @param os The output stream.
     * @param nan The NaN instance.
     * @return The modified output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const NaN&) {
        os << "";
        return os;
    }

    /**
     * @brief Equality comparison operator.
     *
     * @tparam T The type to compare against.
     * @param nan The NaN instance.
     * @param value The value to compare.
     * @return `false` (NaN is not equal to any other value).
     */
    template <typename T>
    friend bool operator==(const NaN&, const T&) {
        return false;
    }

    /**
     * @brief Equality comparison operator for NaN objects.
     *
     * @param nan1 The first NaN instance.
     * @param nan2 The second NaN instance.
     * @return `true` (NaN is equal to itself).
     */
    friend bool operator==(const NaN&, const NaN&) { return true; }

    /**
     * @brief Inequality comparison operator.
     *
     * @tparam T The type to compare against.
     * @param nan The NaN instance.
     * @param value The value to compare.
     * @return `true` (NaN is not equal to any other value).
     */
    template <typename T>
    friend bool operator!=(const NaN&, const T&) {
        return true;
    }

    /**
     * @brief Inequality comparison operator for NaN objects.
     *
     * @param nan1 The first NaN instance.
     * @param nan2 The second NaN instance.
     * @return `false` (NaN is equal to itself).
     */
    friend bool operator!=(const NaN&, const NaN&) { return false; }

    /**
     * @brief Less-than-or-equal-to comparison operator.
     *
     * @tparam T The type to compare against.
     * @param nan The NaN instance.
     * @param value The value to compare.
     * @return `false` (NaN is not less than or equal to any value).
     */
    template <typename T>
    friend bool operator<=(const NaN&, const T&) {
        return false;
    }

    /**
     * @brief Greater-than-or-equal-to comparison operator.
     *
     * @tparam T The type to compare against.
     * @param nan The NaN instance.
     * @param value The value to compare.
     * @return `false` (NaN is not greater than or equal to any value).
     */
    template <typename T>
    friend bool operator>=(const NaN&, const T&) {
        return false;
    }

    /**
     * @brief Less-than comparison operator.
     *
     * @tparam T The type to compare against.
     * @param nan The NaN instance.
     * @param value The value to compare.
     * @return `false` (NaN is not less than any value).
     */
    template <typename T>
    friend bool operator<(const NaN&, const T&) {
        return false;
    }

    /**
     * @brief Greater-than comparison operator.
     *
     * @tparam T The type to compare against.
     * @param nan The NaN instance.
     * @param value The value to compare.
     * @return `false` (NaN is not greater than any value).
     */
    template <typename T>
    friend bool operator>(const NaN&, const T&) {
        return false;
    }
};

}  // namespace cdf

/**
 * @brief A variant type to represent different data types.
 *
 * `_cdfVal` can hold one of the following types:
 * - `std::string`
 * - `int`
 * - `double`
 * - `cdf::NaN`
 *
 * This type is useful for scenarios where the data type is not known in advance
 * and may vary dynamically. The inclusion of `cdf::NaN` allows handling of missing
 * or undefined values in computations.
 */
using _cdfVal = std::variant<std::string, int, double, cdf::NaN>;

#endif  // DTYPES_HPP
