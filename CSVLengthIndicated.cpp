/**
 * @file CSVLengthIndicated.cpp
 * @brief Contains functions for converting a CSV file to a length-indicated format and reading length-indicated records.
 * 
 * This file contains implementations of functions that convert a CSV file to a length-indicated file format and
 * read records from a length-indicated binary file.
 * 
 * @author
 * Thomas Hoerger
 * @date
 * October 18 2024
 */

#include "CSVLengthIndicated.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>  // Added this for std::cerr

/**
 * @brief Converts a CSV file into a length-indicated format.
 * 
 * This function reads the contents of a CSV file and writes each record to an output file, 
 * with each field in the record prefixed by its length. The header row is written as-is without any length indicators.
 * 
 * @param csvFileName The name of the input CSV file.
 * @param outputFileName The name of the output file where the length-indicated records will be written.
 * 
 * @note The header row in the CSV file is written without length indications.
 * @note Each field's length is formatted as a two-digit number, padded with zeroes if necessary.
 */

void convertCSVToLengthIndicated(const std::string& csvFileName, const std::string& outputFileName) {
    std::ifstream inputFile(csvFileName);
    std::ofstream outputFile(outputFileName, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Failed to open file(s)." << std::endl;  // Now std::cerr should work properly
        return;
    }

    std::string line;
    bool isFirstRow = true; // Flag to handle the first row (header)

    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string token;
        std::string lengthIndicatedLine;
        bool isFirstToken = true; // Flag to avoid placing a comma before the first token

        // For the first row, output as it is (header) without length indication
        if (isFirstRow) {
            outputFile << line << std::endl; // Output the header row as comma-separated
            isFirstRow = false; // Mark that header row has been processed
            continue;
        }

        // For each field in the subsequent rows, compute the length and append it before the actual value
        while (std::getline(ss, token, ',')) {
            if (!isFirstToken) {
                lengthIndicatedLine += ",";  // Add a comma between fields
            }
            std::stringstream lengthToken;
            lengthToken << std::setw(2) << std::setfill('0') << token.length() << token;
            lengthIndicatedLine += lengthToken.str();  // Add the formatted field to the output line
            isFirstToken = false;
        }

        outputFile << lengthIndicatedLine << std::endl;  // Write the formatted line to the output file
    }

    inputFile.close();
    outputFile.close();
}



/**
 * @brief Reads a length-indicated record from a binary file.
 * 
 * This function reads a single length-indicated record from the provided binary input file stream.
 * It first reads the length of the record, then reads the actual record data as a string.
 * 
 * @param fileStream The input binary file stream from which to read the length-indicated record.
 * @return A string containing the record data.
 * 
 * @note The function assumes the record length is stored as a `size_t` before each record in the binary file.
 */
std::string readLengthIndicatedRecord(std::ifstream &fileStream) {
    size_t recordLength;  ///< Variable to store the length of the record.
    
    // Read the length of the record (stored as a binary value).
    fileStream.read(reinterpret_cast<char *>(&recordLength), sizeof(recordLength));

    std::string record(recordLength, '\0');  ///< Initialize a string to hold the record data.
    // Read the actual record data.
    fileStream.read(&record[0], recordLength);

    return record;  ///< Return the record data as a string.
}
