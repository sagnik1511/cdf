#ifndef DATAFRAME_HPP
#define DATAFRAME_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "data.hpp"
#include "utils.hpp"
#include "viz.hpp"

namespace cdf {

/**
 * @class DataFrame
 * @brief Represents a data structure similar to a table with rows and columns.
 *
 * The DataFrame class allows for operations similar to a DataFrame in Python's Pandas library,
 * including access to specific rows and columns, viewing the shape, and displaying the head and tail of the data.
 */
class DataFrame {
    std::vector<std::string> columns;          /**< Column names in the DataFrame */
    std::map<std::string, int> columnIndexMap; /**< Map to store column names and their respective indices */
    Data data;                                 /**< Data storage object for rows of the DataFrame */

   public:
    /**
     * @brief Constructs a DataFrame with optional data and columns.
     * @param data The data object to initialize the DataFrame with (default is an empty Data).
     * @param columns The column names for the DataFrame (default is an empty vector).
     */
    DataFrame(Data data = {}, std::vector<std::string> columns = {}) : data(data), columns(columns) {
        columnIndexMap.clear();
        for (int i = 0; i < columns.size(); i++) {
            columnIndexMap[columns[i]] = i;
        }
    }

    /**
     * @brief Gets the shape of the DataFrame.
     * @return A pair representing the number of rows and columns.
     */
    std::pair<int, int> shape() { return data.shape(); }

    /**
     * @brief Displays the first few rows of the DataFrame.
     * @param numRows The number of rows to display (default is 5).
     */
    void head(int numRows = 5) {
        int n = std::min(static_cast<size_t>(numRows), data.size());
        std::vector<Row> rows;
        for (int i = 0; i < n; i++) {
            rows.push_back(data[i]);
        }
        tabulate(columns, rows);
    }

    /**
     * @brief Displays the last few rows of the DataFrame.
     * @param numRows The number of rows to display (default is 5).
     */
    void tail(int numRows = 5) {
        int n = std::min(static_cast<size_t>(numRows), data.size());
        std::vector<Row> rows;
        for (int i = data.size() - n; i < data.size(); i++) {
            rows.push_back(data[i]);
        }
        tabulate(columns, rows);
    }

    /**
     * @brief Returns Series object holidng column values
     * @param columnName The name of the corresponding column
     */
    Series operator[](std::string columnName) {
        if (columnIndexMap.find(columnName) == columnIndexMap.end()) {
            throw std::invalid_argument("Column Not present");
        }
        int colIdx = columnIndexMap[columnName];
        std::vector<_cdfVal> column;
        for (int i = 0; i < data.size(); i++) {
            column.push_back(data[i][colIdx]);
        }

        return cdf::Series(column);
    };

    /**
     * @brief Retrieves a dataframe of the expected rows
     * @param filteredIndexes Filtered information to retrieve rows from the Dataframe
     */
    DataFrame operator[](std::vector<bool> filteredIndexes) {
        std::vector<int> indices;
        for (int i = 0; i < filteredIndexes.size(); i++) {
            if (filteredIndexes[i])
                indices.push_back(i);
        }
        return filter(indices);
    }

    /**
     * @brief Retrieves a slice of the DataFrame by row and column index.
     *
     * @param startIndex The starting row index (default is 0).
     * @param endIndex The ending row index (default is -1, which indicates all rows up to the end).
     * @param startColumnName The starting column name for slicing (default is the first column).
     * @param endColumnName The ending column name for slicing (default is the last column).
     * @return A new DataFrame containing the specified slice of data.
     *
     * @throws std::runtime_error If the column names are not found in the DataFrame.
     * @throws std::invalid_argument If the start column index is greater than the end column index.
     * @throws std::out_of_range If the row indices are out of the DataFrame's range.
     */
    const DataFrame iloc(size_t startIndex = 0, size_t endIndex = -1, std::string startColumnName = "",
                         std::string endColumnName = "") {
        if (startColumnName == "") {
            startColumnName = columns[0];
        }
        if (endColumnName == "") {
            endColumnName = columns[columns.size() - 1];
        }

        if (endIndex == -1) {
            endIndex = data.size() - 1;
        }

        if (columnIndexMap.find(startColumnName) == columnIndexMap.end() ||
            columnIndexMap.find(endColumnName) == columnIndexMap.end()) {
            throw std::runtime_error("Column Name Not Found!");
        }

        int startIdx = columnIndexMap[startColumnName];
        int endIdx = columnIndexMap[endColumnName];

        if (startIdx > endIdx) {
            throw std::invalid_argument("Start Column should be at lower index than End Column");
        }

        if (startIndex < 0 || endIndex >= data.size()) {
            throw std::out_of_range("Indices are out of range!");
        }

        Data tmpData(endIdx - startIdx + 1);
        for (int i = startIndex; i <= endIndex; i++) {
            Row _row = data[i];
            std::vector<_cdfVal> tmpRow;
            for (int j = startIdx; j <= endIdx; j++) {
                tmpRow.push_back(_row[j]);
            }
            Row newRow(tmpRow);
            tmpData.push_back(newRow);
        }

        return DataFrame(tmpData, slice(columns, startIdx, endIdx - startIdx + 1));
    }

    /**
     * @brief Filters rows based on specified row indices.
     *
     * @param indexes A vector of row indices to filter.
     * @return A new DataFrame containing only the rows specified by the indices.
     *
     * @throws std::out_of_range If any of the indices are out of range of the DataFrame.
     */
    const DataFrame filter(const std::vector<int>& indexes) {
        Data tmpData(columns.size());
        for (auto idx : indexes) {
            if (idx < 0 || idx >= data.size()) {
                throw std::out_of_range("Index is out of range!");
            }
            Row _row = data[idx];
            tmpData.push_back(_row);
        }
        return DataFrame(tmpData, columns);
    }
};

}  // namespace cdf

#endif
