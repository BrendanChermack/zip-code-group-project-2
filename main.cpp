#include "CSVProcessing.h"
#include "buffer.h"
#include "CSVLengthIndicated.h"  // Include for the length-indicated conversion
#include <iostream>
#include <fstream>
using namespace std;

void csvConvert_sort(CSVProcessing origin, string file)
{
    cout << "Generating header row." << endl;
    origin.addHeader(file);  // Generate the header for the CSV file
    cout << "Checking for errors" << endl << "Errors: ";

    // Check if the CSV output is successfully generated
    if (origin.csvOutput(file)) {
        cout << "No" << endl << "File made!";
    } else {
        cout << "Yes" << endl << "File not made.";
    }
}

int main() {
    CSVProcessing csvProcessor;
    std::string csvFileName = "us_postal_codes.csv"; // Input CSV file
    std::string outputFileName = "output.csv"; // Processed CSV output file

    // Step 1: Convert and sort CSV
    csvConvert_sort(csvProcessor, outputFileName);

    // Step 2: Convert CSV file to length-indicated format
    cout << "\nConverting CSV to length-indicated format." << endl;
    std::string lengthIndicatedFileName = "us_postal_codes_length_indicated.dat";
    convertCSVToLengthIndicated(csvFileName, lengthIndicatedFileName);
    cout << "CSV file converted to length-indicated format." << endl;

    // Step 3: Read from length-indicated file using Buffer class
    cout << "\nReading from length-indicated file:" << endl;
    std::ifstream inputFile(lengthIndicatedFileName, std::ios::binary);
    Buffer buffer;
    ZipCodeRecord record;

    // Unpack and display the records from the length-indicated file
    while (buffer.readLengthIndicatedRecord(inputFile, record)) {
        cout << "Zip Code: " << record.zip_code << ", City: " << record.city
             << ", State: " << record.state_id << ", Latitude: " << record.latitude
             << ", Longitude: " << record.longitude << endl;
    }

    inputFile.close();

    return 0;
}
