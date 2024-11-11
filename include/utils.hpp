#ifndef UTILS_HPP
#define UTILS_HPP

#include<iostream>
#include<vector>
#include<type_traits>
#include "dtypes.hpp"

void printVariant(const _value& var) {
    std::visit([](auto&& value) {
        std::cout << value;
    }, var);
}

// Template function to print a row of values
template <typename T>
void printRow(const std::vector<T>& row) {
    for (size_t i = 0; i < row.size(); ++i) {
        if constexpr (is_variant<T>::value) {  // Check if T is a variant
            printVariant(row[i]);
        } else {
            std::cout << row[i];
        }

        // Print a comma if it's not the last element
        if (i < row.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "\n";
}


#endif