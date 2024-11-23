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
 * @brief COunt the number of columns in CSV File
 * Reads through unzipped CSV file and counts the number of columns
 *
 * @param csvFilePath absolute of of the CSV File
 * @param delimiter Delimiter of the CSV File(defaults to ',')
 * @returns Number of columns of the CSV File
 */
int countFieldsCSV(std::string& csvFilePath, char delimiter = ',') {
    std::ifstream file(csvFilePath);
    if (!file.is_open()) {
        std::cerr << "Unable to read " << csvFilePath << " !" << std::endl;
        return -1;
    }

    std::string line;
    if (std::getline(file, line)) {  // Read the first line
        size_t i = 0;
        int columnCount = 0;
        bool inQuotes = false;

        while (i < line.size()) {
            if (line[i] == '"') {
                inQuotes = !inQuotes;  // Toggle inQuotes
            } else if (line[i] == delimiter && !inQuotes) {
                columnCount++;  // Increment on unquoted comma
            }
            i++;
        }

        // Add 1 to column count for the last field (or trailing empty field)
        return columnCount + 1;
    }

    return 0;  // Return 0 if file is empty
}

/**
 * @brief Reads a CSV file and loads data into a cdf::DataFrame. (Old Implementation)
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
DataFrame read_csv_old(std::string csvFilePath, char delimiter = ',', int header = 0,
                       std::vector<std::string> names = {}) {
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

    core::Data data;
    std::string line;
    int currIdx = 0;

    bool inQuotes = false;
    std::string quotedString = "";
    // Read the file line by line
    while (std::getline(csvFile, line)) {
        if (currIdx == header) {
            // Process the header line
            std::string val;
            std::stringstream headerStream(line);
            while (std::getline(headerStream, val, delimiter)) {
                headers.push_back(val);
            }
            data = core::Data(headers.size());
        } else {
            // Process each data row
            std::string val;
            std::stringstream rowStream(line);
            std::vector<_cdfVal> cacheRow;
            while (std::getline(rowStream, val, delimiter)) {
                // Handling added whenever any value of the CSV file carries ',' character
                // Whenever a field value contains ',', the field is contained with ""
                // So, whenever a quted character is found, we concatenate the responses from the
                // string split responses until end quote comes
                if (val[0] == '"' && val[val.size() - 1] != '"') {
                    inQuotes = true;
                    quotedString += val.substr(1);
                } else if (inQuotes) {
                    quotedString += "," + val;
                    if (val[val.size() - 1] == '"') {
                        inQuotes = false;
                        val = quotedString.substr(0, quotedString.size() - 1);
                        quotedString = "";
                    }
                }

                if (!inQuotes) {
                    cacheRow.push_back(inferAndConvert(val).second);
                }
            }
            if (line[line.size() - 1] == ',') {
                cacheRow.push_back("");
            }
            core::Row row(cacheRow);
            data.push_back(row);
        }
        ++currIdx;
    }

    // Create and return DataFrame
    DataFrame df = DataFrame(data, headers);
    return df;
}

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
    int numColumns;
    NaN nan = NaN();
    // Store a cache loading the field values before going for type-conversion
    std::vector<std::vector<std::string>> cache;

    std::vector<std::string> headers;
    if (names.size() > 0) {
        header = -1;
        headers = names;
        numColumns = names.size();
    } else {
        // Data-Type Definitions
        numColumns = countFieldsCSV(csvFilePath, delimiter);
    }
    std::vector<int> fieldTypes(numColumns, 0);

    // Open the filepath
    std::ifstream csvFile(csvFilePath);

    // Check if the file can be opened
    if (!csvFile.is_open()) {
        std::cerr << "Unable to load " << csvFilePath << " !" << std::endl;
        return DataFrame();
    }

    std::string line;
    int currIdx = 0;
    bool inQuotes = false;
    std::string quotedString = "";

    // Data Type Vectors to find actual data-type of the columns
    // std::vector<std::string> dTypeRank[3] =

    // Parsing through CSV File and storing values as string in a cache
    while (std::getline(csvFile, line)) {
        std::string val;
        std::stringstream valueStream(line);

        if (currIdx == header) {
            while (std::getline(valueStream, val, delimiter)) {
                headers.push_back(val);
            }
        } else if (currIdx >= 0) {
            std::vector<std::string> rowVector;

            // Parse through Row Values
            while (std::getline(valueStream, val, delimiter)) {
                // Handling added whenever any value of the CSV file carries ',' character
                // Whenever a field value contains ',', the field is contained with ""
                // So, whenever a quted character is found, we concatenate the responses from the
                // string split responses until end quote comes
                if (val[0] == '"' && val[val.size() - 1] != '"') {
                    inQuotes = true;
                    quotedString += val.substr(1);
                } else if (inQuotes) {
                    quotedString += "," + val;
                    if (val[val.size() - 1] == '"') {
                        inQuotes = false;
                        val = quotedString.substr(0, quotedString.size() - 1);
                        quotedString = "";
                    }
                }
                if (!inQuotes) {
                    rowVector.push_back(val);

                    if (val != "") {
                        // Updating Data Type if current data type doesn't matches with returned response
                        // Priority of the data-types -> string > double > int
                        std::pair<int, _cdfVal> inferredData = inferAndConvert(val);
                        if (fieldTypes[rowVector.size() - 1] < inferredData.first) {
                            fieldTypes[rowVector.size() - 1] = inferredData.first;
                        }
                    }
                }
            }
            if (line[line.size() - 1] == ',') {
                val = "";
                rowVector.push_back(val);
            }
            cache.push_back(rowVector);
        }
        ++currIdx;
    }

    // // Print data-type for each fields
    // for (int i = 0; i < headers.size(); i++) {
    //     std::cout << fieldTypes[i] << " " << headers[i] << " : " << dTypeWithRank[fieldTypes[i]] << "\n";
    // }

    // Insert data into Data class after updating data-type
    core::Data data(headers.size());

    for (int i = 0; i < cache.size(); i++) {
        std::vector<_cdfVal> cacheRow;
        for (int j = 0; j < headers.size(); j++) {
            if (cache[i][j] == "") {
                // Pushing nan in place of blank string
                cacheRow.push_back(nan);
            } else if (fieldTypes[j] == 0) {
                // Integer Conversion
                cacheRow.push_back(std::stoi(cache[i][j]));
            } else if (fieldTypes[j] == 1) {
                // Double Conversion
                cacheRow.push_back(std::stod(cache[i][j]));
            } else if (fieldTypes[j] == 2) {
                // String, No Conversion
                cacheRow.push_back(cache[i][j]);
            }
        }
        data.push_back(cacheRow);
    }

    // Load Data into a dataframe
    DataFrame df = DataFrame(data, headers);

    return df;
};

}  // namespace io

}  // namespace cdf

#endif
