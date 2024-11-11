#ifndef DATA_HPP
#define DATA_HPP

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include "utils.hpp"
#include "dtypes.hpp"

namespace cdf {

    // Row Class
    // Abstracted class for row-wise manipulation of data
    class Row{

        public:
        std::vector<_value> row;

        // Constructor Method
        Row(std::vector<std::string> inputVector = {}) {
            for(auto &el : inputVector){
                row.push_back(_value(el));
            }
        };

        // Size of the Row
        size_t size(){
            return row.size();
        }

        // Overload operator[] for non-const access
        _value& operator[](size_t index) {
            if (index >= row.size()) {
                throw std::out_of_range("Index out of range");
            }
            return row[index];
        }

        // Overload operator[] for const access
        const _value& operator[](size_t index) const {
            if (index >= row.size()) {
                throw std::out_of_range("Index out of range");
            }
            return row[index];
        }
        auto begin() { return row.begin(); }
        auto end() { return row.end(); }
        auto begin() const { return row.begin(); }
        auto end() const { return row.end(); }

    };

    // Data Class
    // Storage class of dataframe. Abstracts basic data reading and manipulation
    class Data{
        int numValuesPerRow;
        std::map<int, Row> _data;
        int numRows = 0;

        public:

        // Constructor Method
        Data(int rowLength = 0): numValuesPerRow(rowLength) {
            numRows = 0;
            _data.clear();
        }

        // // Overload operator[] for non-const access
        // Row& operator[](size_t index) {
        //     if (index >= _data.size()) {
        //         throw std::out_of_range("Index out of range");
        //     }
        //     return _data[index];
        // }

        // // Overload operator[] for const access
        // const Row& operator[](size_t index) const {
        //     if (index >= _data.size()) {
        //         throw std::out_of_range("Index out of range");
        //     }
        //     return _data[index];
        // }

        // Fetch row by Index
        Row fetch(int rowIndex){
            return _data[rowIndex];
        }
        
        // Add new entry to data
        void push_back(Row row){
            if(row.size() == numValuesPerRow) {
                _data[numRows++] = row;
            }
        }

        // Shape of the data inserted
        std::pair <int, int>shape() const{
            return {numRows, numValuesPerRow};
        }

    };
}


#endif