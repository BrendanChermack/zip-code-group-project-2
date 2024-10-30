#include "CSVProcessing.h"
#include "buffer.h"
#include "CSVLengthIndicated.h"
#include <iostream>
#include <fstream>
#include "IndexFile.h"
using namespace std;

void csvConvert_sort( CSVProcessing origin, string file ) {
    cout << "Generating header row." << endl;
    origin.addHeader( file );  // Generate the header for the CSV file
    cout << "Checking for errors" << endl << "Errors: ";

    if ( origin.csvOutput( file ) ) {
        cout << "No" << endl << "File made!";
    }
    else {
        cout << "Yes" << endl << "File not made.";
    }
}

std::vector<std::string> splitZipLine(const std::string& str) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find("-z", start);
    
    // Skip the first empty part if string starts with "-z"
    if (start == end) {
        start += 2;  // length of "-z"
        end = str.find("-z", start);
    }
    
    while (end != std::string::npos) {
        // Add the part between current position and next "-z"
        if (end - start > 0) {
            result.push_back(str.substr(start, end - start));
        }
        start = end + 2;  // Skip over "-z"
        end = str.find("-z", start);
    }
    
    // Add the last part if there's anything left
    if (start < str.length()) {
        result.push_back(str.substr(start));
    }
    
    return result;
}

std::string getRecordAtOffset(const std::string& filename, int offset) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open data file.");
    }
	offset = offset;
    file.seekg(offset);  // Jump to the ASCII character position
    std::string line;
   std::getline(file, line);
    file.close();
    return line;
}


void check( const std::string& str, const std::string& outputfile, const std::string& indexName){
	bool notfound = true;
	std::string correct_line;
	 std::ifstream file2(indexName); // Open the file add name later
    if (!file2.is_open()) {
        std::cerr << "Error opening file: index.txt " << std::endl;
        return;
    }
	cout << "str is: " << str << endl;
	std::string strcopy = str;
//	while (strcopy.length() > 0 && strcopy[0] == '0') {
  //      strcopy.erase(0, 1);
    //}
	std::string length, zipcode; //Current word of the index being looked at
    //The length to be skipped to find the proper line
	int i = 0;
	
    while ((file2 >> zipcode >> length) && !file2.eof()) {  // reads word by word
        if(zipcode == str){
			int offset = std::stoi(length);
			cout << "Offset is: " << offset<< endl;

			correct_line = getRecordAtOffset(outputfile, offset+i);
			cout << correct_line<< endl;
			notfound = false;
			break;
			}
			else{
				i++;
				
			}
			
			}
			
	if(notfound){
			cout<< str << " was not found in the index.";
			}
			file2.close();
}


int main() {
    CSVProcessing csvProcessor;
    std::string csvFileName1 = "us_postal_codes.csv";              // Input CSV file 1
    std::string csvFileName2 = "us_postal_codes_ROWS_RANDOMIZED.csv";  // Input CSV file 2
    std::string outputFileName1 = "output1.csv";                   // Output CSV file 1
    std::string outputFileName2 = "output2.csv";                   // Output CSV file 2

    // Step 1: Convert and sort both CSV files
    cout << "Processing and sorting both CSV files." << endl;
    csvConvert_sort( csvProcessor, outputFileName1 );
    csvConvert_sort( csvProcessor, outputFileName2 );

    // Step 2: Convert both CSV files to length-indicated format (ASCII)
    cout << "\nConverting both CSVs to length-indicated format (ASCII)." << endl;
    std::string lengthIndicatedFileName1 = "us_postal_codes_length_indicated.csv";  // Using .txt for ASCII output
    std::string lengthIndicatedFileName2 = "us_postal_codes_RANDOMIZED_length_indicated.csv";  // Using .txt for ASCII output
    convertCSVToLengthIndicated( csvFileName1, lengthIndicatedFileName1 );
    convertCSVToLengthIndicated( csvFileName2, lengthIndicatedFileName2 );
    cout << "Both CSV files converted to length-indicated ASCII format." << endl;
	cout << "Please enter the zip codes you want information about!" << endl;
	std::string text;
	cin >> text;
    auto result = splitZipLine(text);
    
    // Print results
	IndexFile iF;
	std::string indexName = "index2.txt";
    iF.createIndexFile( "us_postal_codes_length_indicated.csv", indexName );
    for (const auto& str : result) {
        //std::cout << str << std::endl;
		string output1 = "us_postal_codes_length_indicated.csv";
		string output2 = "us_postal_codes_RANDOMIZED_length_indicated.csv";
		//check(str,output1);
		check(str,output1, indexName );
    }
    
    return 0;
}
