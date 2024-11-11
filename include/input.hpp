#ifndef INPUT_HPP
#define INPUT_HPP


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "dtypes.hpp"
#include "data.hpp"
#include "dataframe.hpp"

namespace cdf {
    namespace io{

        // Reads a CSV file and loads data into a cdf::DataFrame
        DataFrame read_csv(std::string csvFilePath, char delimiter = ',', int header = 0, std::vector<std::string> names = {}) {
            std::vector<std::string> headers;
            if(names.size() > 0){
                headers = names;
                header = -1;
            }
            std::ifstream csvFile(csvFilePath);
            if(!csvFile.is_open()){
                std::cerr << "Unable to load "<< csvFilePath << " !" << std::endl;
                return DataFrame();
            }
            Data data;
            std::string line;
            int currIdx = 0;
            while(std::getline(csvFile, line)){
                if(currIdx == header){
                    std::string val;
                    std::stringstream headerStream(line);
                    while(std::getline(headerStream, val, delimiter)){
                        headers.push_back(val);
                    }
                    data = Data(headers.size());
                }
                else{
                    std::string val;
                    std::stringstream rowStream(line);
                    std::vector<std::string>cacheRow;
                    while(std::getline(rowStream, val, delimiter)){
                        cacheRow.push_back(val);
                    }
                    Row row = Row(cacheRow);
                    data.push_back(row);
                    }
                ++currIdx;
                }
                DataFrame df = DataFrame(data, headers);

                return df;
        }
    }

}

#endif