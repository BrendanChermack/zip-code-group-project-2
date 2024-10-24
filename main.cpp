#include "CSVProcessing.h"
#include "buffer.h"
#include "CSVLengthIndicated.h"
#include <iostream>
#include <fstream>
using namespace std;

void csvConvert_sort(CSVProcessing origin, string file)
{
    cout << "Generating header row." << endl;
    origin.addHeader(file);  // Generate the header for the CSV file
    cout << "Checking for errors" << endl << "Errors: ";

    if (origin.csvOutput(file)) {
        cout << "No" << endl << "File made!";
    } else {
        cout << "Yes" << endl << "File not made.";
    }
}

int main() {
    CSVProcessing csvProcessor;
    std::string csvFileName1 = "us_postal_codes.csv";              // Input CSV file 1
    std::string csvFileName2 = "us_postal_codes_ROWS_RANDOMIZED.csv";  // Input CSV file 2
    std::string outputFileName1 = "output1.csv";                   // Output CSV file 1
    std::string outputFileName2 = "output2.csv";                   // Output CSV file 2

    // Step 1: Convert and sort both CSV files
    cout << "Processing and sorting both CSV files." << endl;
    csvConvert_sort(csvProcessor, outputFileName1);
    csvConvert_sort(csvProcessor, outputFileName2);

    // Step 2: Convert both CSV files to length-indicated format (ASCII)
    cout << "\nConverting both CSVs to length-indicated format (ASCII)." << endl;
    std::string lengthIndicatedFileName1 = "us_postal_codes_length_indicated.txt";  // Using .txt for ASCII output
    std::string lengthIndicatedFileName2 = "us_postal_codes_RANDOMIZED_length_indicated.txt";  // Using .txt for ASCII output
    convertCSVToLengthIndicated(csvFileName1, lengthIndicatedFileName1);
    convertCSVToLengthIndicated(csvFileName2, lengthIndicatedFileName2);
    cout << "Both CSV files converted to length-indicated ASCII format." << endl;

    // Step 3: Read from the first length-indicated file using Buffer class
    cout << "\nReading from length-indicated file (1):" << endl;
    std::ifstream inputFile1(lengthIndicatedFileName1);
    Buffer buffer;
    ZipCodeRecord record;

    // Unpack and display the records from the first length-indicated file
    while (buffer.readLengthIndicatedRecord(inputFile1, record)) {
        cout << "Zip Code: " << record.zip_code << ", City: " << record.city
             << ", State: " << record.state_id << ", Latitude: " << record.latitude
             << ", Longitude: " << record.longitude << endl;
    }
    inputFile1.close();

    return 0;
}
