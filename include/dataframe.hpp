#ifndef DATAFRAME_HPP
#define DATAFRAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "data.hpp"
#include "viz.hpp"


namespace cdf {

    // CDF - C++ DataFrame Class for Data Manipulation
    class DataFrame{
        std::vector<std::string> columns;
        std::map<std::string, int>colNumMap;
        Data data;
        public:
        std::pair<int, int> shape;

        // Constructor Method
        DataFrame(Data data = Data(0), std::vector<std::string> columns = {}) : data(data), columns(columns) {
            shape = data.shape();
            for(int i=0;i<columns.size();i++){
                colNumMap[columns[i]] = i;
            }
        };

        // Prints first n rows
        void head(int numRows = 5){
            std::vector<Row> printable;
            for(int i=0; i<numRows; i++){
                printable.push_back(data.fetch(i));
            }
            printTable(columns, printable);
        }

        // Prints last n rows
        void tail(int numRows = 5){
            std::vector<Row> printable;
            for(int i=1; i<=numRows; i++){
                printable.push_back(data.fetch(shape.first - i));
            }
            printTable(columns, printable);
        }

        DataFrame loc(std::string columnName, int startIdx, int endIdx){
            if(startIdx > endIdx && startIdx < 0 && endIdx > shape.first){
                std::cerr << "Bad indexing!" << "\n";
                std::cout << "1\n";
                return DataFrame();
            }
            if(colNumMap.find(columnName) == colNumMap.end()){
                std::cerr << "Column Not found!" << "\n";
                std::cout << "2\n";
                return DataFrame();
            }
            int colIdx = colNumMap[columnName];
            std::vector<std::string> tmpCols = {columnName};
            Data tmpData(tmpCols.size());
            for(int i=startIdx;i<endIdx;i++){
                //Row row = Row{data[i][colIdx]};
                tmpData.push_back(data.fetch(i));
            }

            return DataFrame(tmpData, tmpCols);
        }


    };

}
#endif