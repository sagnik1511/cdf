#ifndef INPUT_HPP
#define INPUT_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "data.hpp"
#include "dataframe.hpp"
#include "dtypes.hpp"
#include "utils.hpp"

namespace cdf {
namespace io {

/**
 * @brief Reads a CSV file and loads data into a cdf::DataFrame.
 *
 * This function opens a CSV file, processes its content, and converts it into a `DataFrame` object.
 * It handles headers, row parsing, and converts data types as necessary based on the file contents.
 *
 * @param csvFilePath The path to the CSV file to be loaded.
 * @param delimiter The delimiter used to separate columns (default is comma `,`).
 * @param header The line number that contains the column headers. If `header` is -1, no headers are read from the file.
 * @param names A vector of column names to use if `header` is specified as -1, otherwise it uses the first line as
 * headers.
 * @return A `DataFrame` object containing the data read from the CSV file.
 * @throws std::ios_base::failure if the file cannot be opened.
 */
DataFrame read_csv(std::string csvFilePath, char delimiter = ',', int header = 0, std::vector<std::string> names = {}) {
    std::vector<std::string> headers;

    // If column names are provided, use them directly and disable header reading
    if (names.size() > 0) {
        headers = names;
        header = -1;
    }

    std::ifstream csvFile(csvFilePath);

    // Check if the file can be opened
    if (!csvFile.is_open()) {
        std::cerr << "Unable to load " << csvFilePath << " !" << std::endl;
        return DataFrame();
    }

    Data data;
    std::string line;
    int currIdx = 0;

    // Read the file line by line
    while (std::getline(csvFile, line)) {
        if (currIdx == header) {
            // Process the header line
            std::string val;
            std::stringstream headerStream(line);
            while (std::getline(headerStream, val, delimiter)) {
                headers.push_back(val);
            }
            data = Data(headers.size());
        } else {
            // Process each data row
            std::string val;
            std::stringstream rowStream(line);
            std::vector<_cdfVal> cacheRow;
            while (std::getline(rowStream, val, delimiter)) {
                cacheRow.push_back(inferAndConvert(val));
            }
            Row row = Row(cacheRow);
            data.push_back(row);
        }
        ++currIdx;
    }

    // Create and return DataFrame
    DataFrame df = DataFrame(data, headers);
    return df;
}

}  // namespace io

}  // namespace cdf

#endif
