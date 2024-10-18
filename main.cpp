#include "CSVProcessing.h"
#include "buffer.h"
#include <iostream>
using namespace std;

void csvConvert_sort(CSVProcessing origin, string file)
{
	cout << "Generating header row." << endl;
    origin.addHeader(file);
	cout << "Checking for errors" << endl << "Errors: ";
	
	if (origin.csvOutput(file)) {
		cout << "No" << endl << "File made!";
	} else {
		cout << "Yes" << endl << "File not made.";
	}
	
};





int main() {
    CSVProcessing csvProcessor;

    std::string file_name = "output.csv";
	csvConvert_sort(csvProcessor, file_name);
    //csvProcessor.sortBuffer();

	
	


    return 0;
}