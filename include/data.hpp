#ifndef DATA_HPP
#define DATA_HPP

#include <vector>

#include "dtypes.hpp"

namespace cdf {

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
 * @brief Represents a series of data values.
 *
 * The Series class is a container for a sequence of data points of type `T` and supports element-wise comparison
 * with values of another type `U` using overloaded comparison operators.
 *
 * @tparam T The data type of the series elements.
 */
template <typename T>
class Series {
    std::vector<T> series;

    /**
     * @brief Compares an element with a value using a specified operation.
     *
     * Handles cases where `T` is a variant type and determines the correct type-based comparison.
     *
     * @tparam U The type of the value to compare against.
     * @param rowVal The element of the series to compare.
     * @param value The value to compare with.
     * @param op The comparison operation as a lambda function.
     * @return The result of the comparison.
     */
    template <typename U>
    bool compareVariant(const T& rowVal, const U& value, const std::function<bool(const U&, const U&)>& op) const {
        if constexpr (std::is_same_v<T, _cdfVal>) {
            if (std::holds_alternative<int>(rowVal)) {
                return op(std::get<int>(rowVal), value);
            } else if (std::holds_alternative<double>(rowVal)) {
                return op(std::get<double>(rowVal), value);
            } else if (std::holds_alternative<std::string>(rowVal)) {
                if constexpr (std::is_same_v<U, std::string>) {
                    return op(std::get<std::string>(rowVal), value);
                } else {
                    return false;  // Cannot compare string with non-string
                }
            }
        }
        return false;  // Default for non-variant types
    }

    /**
     * @brief Generic comparison function for the series.
     *
     * Iterates through the series and applies the given comparison operation.
     *
     * @tparam U The type of the value to compare against.
     * @param value The value to compare each series element with.
     * @param op The comparison operation as a lambda function.
     * @return A vector of booleans indicating the result of the comparison for each element in the series.
     */
    template <typename U>
    std::vector<bool> compare(const U& value, const std::function<bool(const U&, const U&)>& op) const {
        std::vector<bool> truth;
        for (const auto& rowVal : series) {
            if constexpr (std::is_same_v<T, _cdfVal>) {
                truth.push_back(compareVariant(rowVal, value, op));
            } else {
                truth.push_back(op(static_cast<U>(rowVal), value));
            }
        }
        return truth;
    }

   public:
    /**
     * @brief Constructs a Series object from a vector of data values.
     *
     * @param series A vector of data values to populate the series with.
     */
    Series(std::vector<T> series) : series(series) {};

    /**
     * @brief Compares each element in the series with a value using the less-than operator.
     */
    template <typename U>
    std::vector<bool> operator<(const U& value) const {
        return compare<U>(value, [](const U& a, const U& b) { return a < b; });
    }

    /**
     * @brief Compares each element in the series with a value using the greater-than operator.
     */
    template <typename U>
    std::vector<bool> operator>(const U& value) const {
        return compare<U>(value, [](const U& a, const U& b) { return a > b; });
    }

    /**
     * @brief Compares each element in the series with a value using the less-than-or-equal-to operator.
     */
    template <typename U>
    std::vector<bool> operator<=(const U& value) const {
        return compare<U>(value, [](const U& a, const U& b) { return a <= b; });
    }

    /**
     * @brief Compares each element in the series with a value using the greater-than-or-equal-to operator.
     */
    template <typename U>
    std::vector<bool> operator>=(const U& value) const {
        return compare<U>(value, [](const U& a, const U& b) { return a >= b; });
    }

    /**
     * @brief Compares each element in the series with a value using the equality operator.
     */
    template <typename U>
    std::vector<bool> operator==(const U& value) const {
        return compare<U>(value, [](const U& a, const U& b) { return a == b; });
    }

    /**
     * @brief Compares each element in the series with a value using the inequality operator.
     */
    template <typename U>
    std::vector<bool> operator!=(const U& value) const {
        return compare<U>(value, [](const U& a, const U& b) { return a != b; });
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
};
}  // namespace cdf

#endif
