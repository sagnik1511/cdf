#ifndef DATA_HPP
#define DATA_HPP

#include <vector>

#include "dtypes.hpp"

namespace cdf {

namespace core {

/**
 * @brief Represents a single row of data.
 *
 * The Row class stores a collection of data values of type `_cdfVal`, providing access to individual elements
 * via the subscript operator and the ability to check its size.
 */
class Row {
    std::vector<_cdfVal> _row;

   public:
    /**
     * @brief Constructs a Row object from a vector of data.
     *
     * @param inputRow A vector of data values to populate the row with.
     */
    template <typename T>
    Row(std::vector<T> inputRow) {
        for (auto& it : inputRow) {
            _row.push_back(it);
        }
    }

    /**
     * @brief Returns the size of the row.
     *
     * @return The number of elements in the row.
     */
    size_t size() { return _row.size(); }

    /**
     * @brief Accesses the element at the specified index in the row.
     *
     * @param index The index of the element to access.
     * @return A constant reference to the element at the given index.
     * @throws std::out_of_range if the index is out of bounds.
     */
    const _cdfVal& operator[](size_t index) const {
        if (index >= _row.size()) {
            throw std::out_of_range("Index out of range!");
        }
        return _row[index];
    }
};

/**
 * @class Series
 * @brief A class that represents a series of heterogeneous data values and provides comparison utilities.
 *
 * The Series class stores a sequence of values of type `_cdfVal` (which can hold multiple types, such as int, double,
 * or std::string) and allows comparisons between the series elements and a provided value using standard comparison
 * operators (e.g., ==, <, <=, >, >=, !=).
 */
class Series {
    std::vector<_cdfVal> series;

    /**
     * @brief Compares each string representation of elements in the series with a given string using a custom
     * comparator.
     *
     * @tparam Comparator The type of the comparator function.
     * @param val The string value to compare against.
     * @param op The comparator function to use for the comparison.
     * @return A vector of boolean values indicating the result of the comparison for each element in the series.
     */
    template <typename Comparator>
    std::vector<bool> compareString(const std::string& val, const Comparator& op) const {
        std::vector<bool> truth;
        for (const auto& rowVal : series) {
            truth.push_back(std::visit(
                [&](const auto& v) -> bool {
                    using T = std::decay_t<decltype(v)>;
                    if constexpr (std::is_same_v<T, int>) {
                        return op(std::to_string(v), val);
                    } else if constexpr (std::is_same_v<T, double>) {
                        return op(std::to_string(v), val);
                    } else if constexpr (std::is_same_v<T, std::string>) {
                        return op(v, val);
                    } else {
                        return false;  // Handle cdf::NaN or mismatched types
                    }
                },
                rowVal));
        }
        return truth;
    }

    /**
     * @brief Compares each integer element in the series with a given integer using a custom comparator.
     *
     * @tparam Comparator The type of the comparator function.
     * @param val The integer value to compare against.
     * @param op The comparator function to use for the comparison.
     * @return A vector of boolean values indicating the result of the comparison for each element in the series.
     */
    template <typename Comparator>
    std::vector<bool> compareInt(int val, const Comparator& op) const {
        std::vector<bool> truth;
        for (const auto& rowVal : series) {
            truth.push_back(std::visit(
                [&](const auto& v) -> bool {
                    using T = std::decay_t<decltype(v)>;
                    if constexpr (std::is_same_v<T, int>) {
                        return op(v, val);
                    } else if constexpr (std::is_same_v<T, double>) {
                        return op(v, static_cast<double>(val));
                    } else {
                        return false;  // Handle cdf::NaN, strings, or mismatched types
                    }
                },
                rowVal));
        }
        return truth;
    }

    /**
     * @brief Compares each double element in the series with a given double using a custom comparator.
     *
     * @tparam Comparator The type of the comparator function.
     * @param val The double value to compare against.
     * @param op The comparator function to use for the comparison.
     * @return A vector of boolean values indicating the result of the comparison for each element in the series.
     */
    template <typename Comparator>
    std::vector<bool> compareDouble(double val, const Comparator& op) const {
        std::vector<bool> truth;
        for (const auto& rowVal : series) {
            truth.push_back(std::visit(
                [&](const auto& v) -> bool {
                    using T = std::decay_t<decltype(v)>;
                    if constexpr (std::is_same_v<T, double>) {
                        return op(v, val);
                    } else if constexpr (std::is_same_v<T, int>) {
                        return op(static_cast<double>(v), val);
                    } else {
                        return false;  // Handle cdf::NaN, strings, or mismatched types
                    }
                },
                rowVal));
        }
        return truth;
    }

    /**
     * @brief Compares elements in the series with a given value using a custom comparator.
     *
     * This method routes to the appropriate compare function (compareString, compareInt, or compareDouble) based on
     * the type of the provided value.
     *
     * @tparam T The type of the value to compare.
     * @tparam Comparator The type of the comparator function.
     * @param value The value to compare against.
     * @param op The comparator function to use for the comparison.
     * @return A vector of boolean values indicating the result of the comparison for each element in the series.
     */
    template <typename T, typename Comparator>
    std::vector<bool> compare(const T& value, const Comparator& op) {
        if constexpr (std::is_same_v<T, std::string>) {
            return compareString(value, op);
        } else if constexpr (std::is_same_v<T, int>) {
            return compareInt(value, op);
        } else if constexpr (std::is_same_v<T, double>) {
            return compareDouble(value, op);
        } else {
            std::cout << "Unknown data-type found\n";
            std::vector<bool> truth(series.size(), false);
            return truth;
        }
    }

   public:
    /**
     * @brief Constructs a Series object from a vector of data values.
     *
     * @param series A vector of data values to populate the series with.
     */
    Series(std::vector<_cdfVal> series) : series(series) {};

    /**
     * @brief Equality comparison operator.
     *
     * Compares each element in the series with a provided value for equality.
     *
     * @tparam T The type of the value to compare.
     * @param value The value to compare against.
     * @return A vector of boolean values indicating equality for each element in the series.
     */
    template <typename T>
    std::vector<bool> operator==(const T& value) {
        return compare(value, std::equal_to<>{});
    }

    /**
     * @brief Not-equal-to comparison operator.
     *
     * Compares each element in the series with a provided value for inequality.
     *
     * @tparam T The type of the value to compare.
     * @param value The value to compare against.
     * @return A vector of boolean values indicating inequality for each element in the series.
     */
    template <typename T>
    std::vector<bool> operator!=(const T& value) {
        return compare(value, std::not_equal_to<>{});
    }

    /**
     * @brief Less-than comparison operator.
     *
     * Compares each element in the series with a provided value for "less than".
     *
     * @tparam T The type of the value to compare.
     * @param value The value to compare against.
     * @return A vector of boolean values indicating the result of "less than" for each element in the series.
     */
    template <typename T>
    std::vector<bool> operator<(const T& value) {
        return compare(value, std::less<>{});
    }

    /**
     * @brief Less-than-or-equal-to comparison operator.
     *
     * Compares each element in the series with a provided value for "less than or equal to".
     *
     * @tparam T The type of the value to compare.
     * @param value The value to compare against.
     * @return A vector of boolean values indicating the result of "less than or equal to" for each element in the
     * series.
     */
    template <typename T>
    std::vector<bool> operator<=(const T& value) {
        return compare(value, std::less_equal<>{});
    }

    /**
     * @brief Greater-than comparison operator.
     *
     * Compares each element in the series with a provided value for "greater than".
     *
     * @tparam T The type of the value to compare.
     * @param value The value to compare against.
     * @return A vector of boolean values indicating the result of "greater than" for each element in the series.
     */
    template <typename T>
    std::vector<bool> operator>(const T& value) {
        return compare(value, std::greater<>{});
    }

    /**
     * @brief Greater-than-or-equal-to comparison operator.
     *
     * Compares each element in the series with a provided value for "greater than or equal to".
     *
     * @tparam T The type of the value to compare.
     * @param value The value to compare against.
     * @return A vector of boolean values indicating the result of "greater than or equal to" for each element in the
     * series.
     */
    template <typename T>
    std::vector<bool> operator>=(const T& value) {
        return compare(value, std::greater_equal<>{});
    }
};

/**
 * @brief Represents a collection of rows of data, akin to a 2D matrix or dataframe.
 *
 * The Data class stores a collection of Row objects and provides methods to access and manipulate the data,
 * as well as retrieve the shape of the dataset.
 */
class Data {
    std::vector<Row> _data;

   public:
    int rowN, colN;

    /**
     * @brief Constructs a Data object with a specified number of columns.
     *
     * @param rowLength The number of columns in each row.
     */
    Data(int rowLength = 0) {
        rowN = 0;
        colN = rowLength;
    }

    /**
     * @brief Returns the number of rows in the data.
     *
     * @return The number of rows in the dataset.
     */
    size_t size() { return rowN; }

    /**
     * @brief Returns the shape of the dataset as a pair of (rows, columns).
     *
     * @return A pair where the first element is the number of rows and the second is the number of columns.
     */
    std::pair<int, int> shape() { return std::make_pair(rowN, colN); }

    /**
     * @brief Accesses the row at the specified index.
     *
     * @param index The index of the row to access.
     * @return A constant reference to the row at the given index.
     * @throws std::out_of_range if the index is out of bounds.
     */
    const Row& operator[](size_t index) const {
        if (index >= _data.size()) {
            throw std::out_of_range("Index out of range!");
        }
        return _data[index];
    }

    /**
     * @brief Adds a new row to the dataset.
     *
     * @param row The row to add.
     * @throws std::length_error if the size of the row does not match the number of columns.
     */
    void push_back(Row& row) {
        if (row.size() == colN) {
            _data.push_back(row);
            ++rowN;
        } else {
            std::cout << "[Data][push_back] Expected " << colN << " columns, found " << row.size() << "\n";
            throw std::length_error("Row size not matching with column size");
        }
    }

    /**
     * @brief Adds a new row to teh dataset.
     *
     * @param row (Vector of _cdfVal) The row to add.
     * @throws std::length_error if the size of the row does not match the number of columns.
     */
    void push_back(std::vector<_cdfVal>& row) {
        Row _row = Row(row);
        push_back(_row);
    }
};
}  // namespace core

}  // namespace cdf

#endif
