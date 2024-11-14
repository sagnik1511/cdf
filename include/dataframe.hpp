#ifndef DATAFRAME_HPP
#define DATAFRAME_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "data.hpp"
#include "viz.hpp"
#include "utils.hpp"

namespace cdf {

    class DataFrame {
        std::vector<std::string>columns;
        std::map<std::string, int> columnIndexMap;
        Data data;
        public:
        DataFrame(Data data = {}, std::vector<std::string> columns = {}) : data(data), columns(columns) {
            columnIndexMap.clear();
            for(int i=0; i<columns.size();i++){
                columnIndexMap[columns[i]] = i;
            }
        };
        std::pair<int, int> shape(){
            return data.shape();
        }
        
        void head(int numRows=5){
            std::vector<Row> rows;
            for(int i=0; i<numRows; i++){
                rows.push_back(data[i]);
            }
            tabulate(columns, rows);
        }

        void tail(int numRows=5){
            std::vector<Row> rows;
            for(int i=data.size() - numRows; i<data.size(); i++){
                rows.push_back(data[i]);
            }
            tabulate(columns, rows);
        }

        const DataFrame iloc(size_t startIndex = 0, size_t endIndex = -1, std::string startColumnName = "", std::string endColumnName = "") {
            if(startColumnName == ""){
                startColumnName = columns[0];
            }
            if(endColumnName == ""){
                endColumnName = columns[columns.size()-1];
            }

            if(columnIndexMap.find(startColumnName) == columnIndexMap.end() || columnIndexMap.find(endColumnName) == columnIndexMap.end()){
                throw std::runtime_error("Column Name Not Found!");
            }

            int startIdx = columnIndexMap[startColumnName];
            int endIdx = columnIndexMap[endColumnName];

            if(startIdx > endIdx){
                throw std::invalid_argument("Start Column should be at lower index that End Column");
            }

            if(startIndex < 0 || endIndex >= data.size()) {
                throw std::out_of_range("Index are out of range!");
            }
            Data tmpData(endIdx - startIdx + 1);
            std::cout << startIdx << ", " << endIdx - startIdx + 1 << "\n";
            for(int i=startIndex; i<=endIndex; i++){
                Row _row = data[i];
                std::vector<_cdfVal> tmpRow;
                for(int j=startIdx; j<= endIdx; j++){
                    tmpRow.push_back(_row[j]);
                }
                Row newRow(tmpRow);
                tmpData.push_back(newRow);
            }

            return DataFrame(tmpData, slice(columns, startIdx, endIdx - startIdx + 1));
            
        }

    };

}

#endif