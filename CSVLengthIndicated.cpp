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
#include <vector>


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

void convertCSVToLengthIndicated( const std::string& csvFileName, const std::string& outputFileName ) {
    std::ifstream inputFile( csvFileName );
    std::ofstream outputFile( outputFileName );

    if ( !inputFile.is_open() || !outputFile.is_open() ) {
        std::cerr << "Failed to open file(s)." << std::endl;
        return;
    }

    std::string line;
    bool isFirstRow = true;

    while ( std::getline( inputFile, line ) ) {
        // Skip the header row
        if ( isFirstRow ) {
            isFirstRow = false;
            continue;
        }

        std::istringstream ss( line );
        std::string token;
        std::string lengthIndicatedLine;
        bool isFirstToken = true;

        // For each field, compute the length and append it before the actual value
        while ( std::getline( ss, token, ',' ) ) {
            if ( !isFirstToken ) {
                lengthIndicatedLine += ",";  // Add a comma between fields
            }

            // Strip quotation marks if necessary
            if ( !token.empty() && token.front() == '"' && token.back() == '"' ) {
                token = token.substr( 1, token.length() - 2 );
            }

            // Ensure the length does not exceed 99
            if ( token.length() > 99 ) {
                std::cerr << "Field length exceeds two-digit limit: " << token << std::endl;
                token = token.substr( 0, 99 );  // Truncate to 99 characters
            }

            // Convert floating-point numbers to strings with fixed precision
            if ( token.find( '.' ) != std::string::npos && ( token[ 0 ] >= '0' && token[ 0 ] <= '9' || token[ 0 ] == '-' ) ) {
                // It's a number, ensure fixed precision
                double num = std::stod( token );
                std::ostringstream oss;
                oss << std::fixed << std::setprecision( 6 ) << num;
                token = oss.str();
            }

            int fieldLength = token.length();

            // For debugging purposes, print field data and length
            // std::cout << "Field Data: '" << token << "', Length: " << fieldLength << std::endl;

            std::stringstream lengthToken;
            lengthToken << std::setw( 2 ) << std::setfill( '0' ) << fieldLength << token;
            lengthIndicatedLine += lengthToken.str();
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
std::vector<std::vector<std::string>> readCSV( const std::string& filename ) {
    std::vector<std::vector<std::string>> data;  // Outer vector to store all rows

    std::ifstream file( filename );  // Open the file
    if ( !file.is_open() ) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }

    std::string line;
    bool isHeader = true;  // Skip the header row if needed

    // Read the file line by line
    while ( std::getline( file, line ) ) {
        if ( isHeader ) {  // Skip the first header line
            isHeader = false;
            continue;
        }

        std::istringstream ss( line );  // Create a string stream for parsing the line
        std::vector<std::string> row;  // Inner vector to store each row's fields
        std::string field;

        // Parse each field separated by commas
        while ( std::getline( ss, field, ',' ) ) {
            row.push_back( field );  // Add each field to the row vector
        }

        data.push_back( row );  // Add the row to the outer vector
    }

    file.close();  // Close the file
    return data;   // Return the populated data
}
