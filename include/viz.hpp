#ifndef VIZ_HPP
#define VIZ_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <variant>
#include <algorithm>
#include <sstream>
#include "dtypes.hpp"
#include "data.hpp"


namespace cdf{
    // Function to calculate the string length of a variant (for column width calculation)
    size_t variantSize(const _value& var) {
        return std::visit([](auto&& value) -> size_t {
            if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) {
                return value.length();
            } else {
                return std::to_string(value).length();
            }
        }, var);
    }

    // Function to calculate maximum column width for each column in the table
    std::vector<size_t> calculateColumnWidths(const std::vector<std::string>& header, const std::vector<Row>& table) {
        std::vector<size_t> columnWidths(header.size(), 0);

        // Calculate widths based on header
        for (size_t i = 0; i < header.size(); ++i) {
            columnWidths[i] = header[i].length();
        }

        // Calculate widths based on table data
        for (const auto& row : table) {
            for (size_t i = 0; i < row.row.size(); ++i) {
                size_t cellWidth = variantSize(row[i]);
                columnWidths[i] = std::max(columnWidths[i], cellWidth);
            }
        }

        return columnWidths;
    }

    // Function to print a horizontal border based on column widths
    void printHorizontalBorder(const std::vector<size_t>& columnWidths) {
        std::cout << "+";
        for (size_t width : columnWidths) {
            std::cout << std::string(width + 2, '-') << "+";
        }
        std::cout << "\n";
    }

    // Function to print the header row
    void printHeader(const std::vector<std::string>& header, const std::vector<size_t>& columnWidths) {
        std::cout << "|";
        for (size_t i = 0; i < header.size(); ++i) {
            std::cout << " " << std::left << std::setw(columnWidths[i]) << header[i] << " |";
        }
        std::cout << "\n";
    }

    // Function to print each data row in the table
    void printRow(const std::vector<_value>& row, const std::vector<size_t>& columnWidths) {
        std::cout << "|";
        for (size_t i = 0; i < row.size(); ++i) {
            std::visit([&](auto&& value) {
                std::cout << " " << std::left << std::setw(columnWidths[i]);
                if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) {
                    std::cout << value;
                } else {
                    std::cout << std::to_string(value);
                }
                std::cout << " |";
            }, row[i]);
        }
        std::cout << "\n";
    }

    // Function to print the full table with pretty CLI formatting
    void printTable(const std::vector<std::string>& header, const std::vector<Row>& table) {
        if (table.empty()) return;

        // Calculate column widths
        std::vector<size_t> columnWidths = calculateColumnWidths(header, table);

        // Print the top border
        printHorizontalBorder(columnWidths);

        // Print header
        printHeader(header, columnWidths);
        printHorizontalBorder(columnWidths);  // Separator between header and data

        // Print each row with borders and alignment
        for (const auto& row : table) {
            printRow(row.row, columnWidths);
            printHorizontalBorder(columnWidths);  // Print a border after each row
        }
    }
    
}



#endif
