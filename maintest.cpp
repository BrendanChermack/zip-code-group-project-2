#include "CSVProcessing.h"
#include "buffer.h"
#include "CSVLengthIndicated.h" */
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Using find and substr
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

    file.seekg(offset);  // Jump to the ASCII character position
    std::string line;
    std::getline(file, line);  // Read the record at this position
    file.close();
    return line;
}


void check( const std::string& str, std::string& outputfile){
	bool notfound = true;
	std::string correct_line;
	 std::ifstream file2(index.txt); // Open the file add name later
    if (!file.is_open()) {
        std::cerr << "Error opening file: indext.txt " << std::endl;
        return false;
    }
	std::string word; //Current word of the index being looked at
    std::string length; //The length to be skipped to find the proper line
    while ((file2 >> word) && notfound) {  // reads word by word
        if(word == str){
			if(file2>> word){
			length = word;
			int offset = std::stoi(length);
			correct_line = getRecordAtOffset(outputfile, offset);
			cout << correct_line<< endl;
			notfound = false;
			}
			else{
				cout << "File over?"<< endl;
				notfound = false;
			}
			
    }
	if(notfound){
	cout<< str << " was not found in the index.";
	}
}

int main() {
    std::string text = "-z12345-z23456-z32467";
	cout << "Please enter the zip codes you want information about!" << endl;
	cin >> text;
    auto result = splitZipLine(text);
    
    // Print results
    for (const auto& str : result) {
        //std::cout << str << std::endl;
		check(str);
		
		
    }
    
  /*  // If you want to preserve the "-z" prefix in the results:
    std::cout << "\nWith '-z' preserved:\n";
    for (const auto& str : result) {
        std::cout << "-z" << str << std::endl; */
    }
