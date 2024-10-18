/**
 * @file CSVLengthIndicated.h
 * @brief Header file for functions to handle length-indicated file conversion and reading.
 * 
 * This header file declares the functions that convert a CSV file into a length-indicated format
 * and read records from a length-indicated binary file. The length-indicated format prefixes each
 * record with the length of the record (in bytes) before the actual data.
 * 
 * @author
 * Thomas Hoerger
 * @date
 * October 18 2024
 */

#ifndef CSV_LENGTH_INDICATED_H
#define CSV_LENGTH_INDICATED_H

#include <string>
#include <fstream>

/**
 * @brief Converts a CSV file to a length-indicated file format.
 * 
 * This function reads a CSV file and writes each record into a binary file format where the length of each 
 * record (in bytes) is stored before the record itself as a comma-separated string. The length is stored 
 * as a binary value, followed by the record data.
 * 
 * @param csvFile The input CSV file path.
 * @param outputFile The output file path where the length-indicated records will be written.
 * 
 * @note The output file will be in binary format with each record prefixed by its length.
 */
void convertCSVToLengthIndicated(const std::string &csvFile, const std::string &outputFile);

/**
 * @brief Reads a single length-indicated record from a file stream.
 * 
 * This function reads a binary file containing length-indicated records and extracts a single record. 
 * The length of the record is read first, followed by the actual record content, which is returned 
 * as a string.
 * 
 * @param fileStream An input file stream from which the length-indicated record will be read.
 * @return A string containing the record data.
 * 
 * @note The function assumes that the file is in the correct binary format, with each record prefixed by its length.
 */
std::string readLengthIndicatedRecord(std::ifstream &fileStream);

#endif // CSV_LENGTH_INDICATED_H
