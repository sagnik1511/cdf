#ifndef VIZ_HPP
#define VIZ_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "data.hpp"
#include "utils.hpp"

namespace cdf {

// Finds maximum length of the character on each column
std::vector<size_t> findMaxLength(std::vector<std::string> headers, std::vector<core::Row> rows) {
    std::vector<size_t> maxLen(headers.size(), 0);

    for (int i = 0; i < headers.size(); i++) {
        maxLen[i] = std::max(maxLen[i], headers[i].size());
        for (auto& row : rows) {
            maxLen[i] = std::max(maxLen[i], toString(row[i]).size());
        }
    }

    return maxLen;
};

// Prints a horizontal line based on the padding
void addHorizontalLine(std::vector<size_t> width) {
    std::cout << "+";
    for (int i = 0; i < width.size(); i++) {
        std::cout << std::string(width[i] + 2, '-');
        std::cout << "+";
    }
    std::cout << "\n";
};

// prints each dataframe row
void printRow(core::Row row, std::vector<size_t> width) {
    std::cout << "|";
    for (int i = 0; i < row.size(); i++) {
        std::string strValue = toString(row[i]);
        std::cout << " ";
        std::cout << strValue;
        std::cout << std::string(width[i] - strValue.size(), ' ');
        std::cout << " |";
    }
    std::cout << "\n";
};

// Prints a whole dataframe view on CLI
void tabulate(std::vector<std::string> headers, std::vector<core::Row> rows) {
    std::vector<size_t> maxLen = findMaxLength(headers, rows);

    addHorizontalLine(maxLen);

    // Add Header Row
    std::cout << "|";
    for (int i = 0; i < headers.size(); i++) {
        std::cout << " ";
        std::cout << headers[i];
        std::cout << std::string(maxLen[i] - headers[i].size(), ' ');
        std::cout << " |";
    }
    std::cout << "\n";
    addHorizontalLine(maxLen);

    // Data
    for (auto& row : rows) {
        printRow(row, maxLen);
        addHorizontalLine(maxLen);
    }
};
}  // namespace cdf

#endif