#ifndef DATAFRAME_HPP
#define DATAFRAME_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "data.hpp"
#include "dtypes.hpp"
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
    std::map<std::string, int> columnIndexMap; /**< Map to store column names and their respective indices */
    core::Data data;                           /**< Data storage object for rows of the DataFrame */

   public:
    std::vector<std::string> columns; /**< Column names in the DataFrame */
    /**
     * @brief Constructs a DataFrame with optional data and columns.
     * @param data The data object to initialize the DataFrame with (default is an empty Data).
     * @param columns The column names for the DataFrame (default is an empty vector).
     */
    DataFrame(core::Data data = {}, std::vector<std::string> columns = {}) : data(data), columns(columns) {
        columnIndexMap.clear();
        for (int i = 0; i < columns.size(); i++) {
            columnIndexMap[columns[i]] = i;
        }
    }

    /**
     * @brief Constructs a DataFrame with row-wise information, column names and data-types.
     * @param inputData Vector of Vector of _cdfVal as row-sie information.
     * @param columns The column names for the DataFrame
     * @param dataTypes The DataType of the columns.
     *
     */
    DataFrame(std::vector<std::vector<_cdfVal>> inputData, std::vector<std::string> columns,
              std::vector<cdfDTypes> dataTypes)
        : columns(columns) {
        // Update columnIndexMap
        columnIndexMap.clear();
        for (int i = 0; i < columns.size(); i++) {
            columnIndexMap[columns[i]] = i;
        }

        // Define a blank Data object
        core::Data tmpData(columns.size());

        // Parse data from inputData and fill as per given data-type
        for (auto& row : inputData) {
            std::vector<_cdfVal> tmpRow;
            for (int i = 0; i < columns.size(); i++) {
                switch (dataTypes[i]) {
                    case cdfDTypes::Integer:
                        tmpRow.push_back(std::stoi(toString(row[i])));
                        break;
                    case cdfDTypes::Double:
                        tmpRow.push_back(std::stod(toString(row[i])));
                        break;
                    case cdfDTypes::String:
                        tmpRow.push_back(toString(row[i]));
                        break;
                }
            }
            tmpData.push_back(tmpRow);
        }

        // Push generated data to actual data object
        data = tmpData;
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
        std::vector<core::Row> rows;
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
        std::vector<core::Row> rows;
        for (int i = data.size() - n; i < data.size(); i++) {
            rows.push_back(data[i]);
        }
        tabulate(columns, rows);
    }

    /**
     * @brief Returns Series object holidng column values
     * @param columnName The name of the corresponding column
     */
    core::Series operator[](std::string columnName) {
        if (columnIndexMap.find(columnName) == columnIndexMap.end()) {
            throw std::invalid_argument("[cdf][DataFrame] Column Not present");
        }
        int colIdx = columnIndexMap[columnName];
        std::vector<_cdfVal> column;
        for (int i = 0; i < data.size(); i++) {
            column.push_back(data[i][colIdx]);
        }

        return cdf::core::Series(column);
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
     * @brief Selects particular columns
     *
     * Creates a slice of columns
     *
     * @param fields A vector of string containing the required columns for the subset
     * @throws std::out_of_range If any column is not presnet in the current dataframe
     *
     * @returns DataFrame consisting of the given columns
     */
    const DataFrame operator[](const std::vector<std::string> fields) {
        std::vector<int> validColumnIndexes;

        // Checks and stores column indexes for further data-gathering
        for (auto field : fields) {
            if (columnIndexMap.find(field) != columnIndexMap.end()) {
                validColumnIndexes.push_back(columnIndexMap[field]);
            } else {
                std::string errorMessage = "[cdf][DataFrame] " + field + " not present inside dataframe object!";
                throw std::out_of_range(errorMessage);
            }
        }

        core::Data tmpData(validColumnIndexes.size());
        for (int i = 0; i < data.size(); i++) {
            std::vector<_cdfVal> tmpRow;
            for (auto& idx : validColumnIndexes) {
                tmpRow.push_back(data[i][idx]);
            }
            tmpData.push_back(tmpRow);
        }

        return DataFrame(tmpData, fields);
    };

    /**
     * @brief Retrieves a slice of the DataFrame by row and column index.
     *
     * @param startRowIndex The starting row index (default is 0).
     * @param endRowIndex The ending row index (default is -1, which indicates all rows up to the end).
     * @param startColumnName The starting column name for slicing (default is the first column).
     * @param endColumnName The ending column name for slicing (default is the last column).
     * @return A new DataFrame containing the specified slice of data.
     *
     * @throws std::runtime_error If the column names are not found in the DataFrame.
     * @throws std::invalid_argument If the start column index is greater than the end column index.
     * @throws std::out_of_range If the row indices are out of the DataFrame's range.
     */
    const DataFrame iloc(size_t startRowIndex = 0, size_t endRowIndex = -1, std::string startColumnName = "",
                         std::string endColumnName = "") {
        if (startColumnName == "") {
            startColumnName = columns[0];
        }
        if (endColumnName == "") {
            endColumnName = columns[columns.size() - 1];
        }

        if (endRowIndex == -1) {
            endRowIndex = data.size() - 1;
        }

        if (columnIndexMap.find(startColumnName) == columnIndexMap.end() ||
            columnIndexMap.find(endColumnName) == columnIndexMap.end()) {
            throw std::runtime_error("[cdf][DataFrame] Column Name Not Found!");
        }

        int startColIdx = columnIndexMap[startColumnName];
        int endColIdx = columnIndexMap[endColumnName];

        if (startColIdx > endColIdx) {
            throw std::invalid_argument("[cdf][DataFrame] Start Column should be at lower index than End Column");
        }

        if (startRowIndex < 0 || endRowIndex >= data.size()) {
            throw std::out_of_range("[cdf][DataFrame] Indices are out of range!");
        }

        core::Data tmpData(endColIdx - startColIdx + 1);
        for (int i = startRowIndex; i <= endRowIndex; i++) {
            core::Row _row(data[i]);
            std::vector<_cdfVal> tmpRow;
            for (int j = startColIdx; j <= endColIdx; j++) {
                tmpRow.push_back(_row[j]);
            }
            core::Row newRow(tmpRow);
            tmpData.push_back(newRow);
        }

        return DataFrame(tmpData,
                         std::vector<std::string>(columns.begin() + startColIdx, columns.begin() + endColIdx + 1));
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
        core::Data tmpData(columns.size());
        for (auto idx : indexes) {
            if (idx < 0 || idx >= data.size()) {
                throw std::out_of_range("[cdf][DataFrame] Index is out of range!");
            }
            core::Row _row = data[idx];
            tmpData.push_back(_row);
        }
        return DataFrame(tmpData, columns);
    }
};

}  // namespace cdf

#endif
