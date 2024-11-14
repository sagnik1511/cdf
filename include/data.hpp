#ifndef DATA_HPP
#define DATA_HPP

#include <vector>
#include "dtypes.hpp"

namespace cdf{

    class Row{
        std::vector<_cdfVal> _row;
        public:

        template<typename T>
        Row(std::vector<T> inputRow){
            for(auto &it : inputRow){
                _row.push_back(it);
            }
        }

        size_t size(){
            return _row.size();
        }

        const _cdfVal& operator[] (size_t index) const {
            if(index >= _row.size()) {
                throw std::out_of_range("Index out of range!");
            }
            return _row[index];
        }
    };


    class Data{
        std::vector<Row> _data;
        public:
        int rowN, colN;
        Data(int rowLength = 0){
            rowN = 0;
            colN = rowLength;
        }

        size_t size(){
            return rowN;
        }
        
        std::pair<int, int> shape(){
            return std::make_pair(rowN, colN);
        }

        const Row& operator[](size_t index) const {
            if(index >= _data.size()) {
                throw std::out_of_range("Index out of range!");
            }
            return _data[index];
        }

        void push_back(Row &row){
            if(row.size() == colN){
                _data.push_back(row);
                ++rowN;
            }
            else{
                std::cout << "Row Size: " << row.size() << " Column Size: " << colN << "\n";
                throw std::length_error("Row size not matching with column size");
            }
        }
    };


}


#endif