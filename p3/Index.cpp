#include "Index.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Helper function to split a string by a delimiter
vector<string> Index::split( const string& line, char delimiter ) {
  vector<string> tokens;
  string token;
  stringstream ss( line );
  while ( getline( ss, token, delimiter ) ) {
    tokens.push_back( token );
  }
  return tokens;
}

// Method to process block data
void Index::processBlockData( const string& inputFileName, const string& outputFileName ) {
  ifstream inputFile( inputFileName );
  if ( !inputFile.is_open() ) {
    cerr << "Error: Could not open " << inputFileName << endl;
    return;
  }

  ofstream outputFile( outputFileName );
  if ( !outputFile.is_open() ) {
    cerr << "Error: Could not open " << outputFileName << endl;
    return;
  }

  outputFile << "Block,Zip Code\n";
  string line;
  getline( inputFile, line );
  line = "";
  while ( getline( inputFile, line ) ) {
    if ( line.empty() ) continue;

    // Check if the line contains a colon; if not, skip it
    size_t colonPos = line.find( ':' );
    if ( colonPos == string::npos ) {
      // If no colon, perform additional validation
      vector<string> fields = split( line, ',' );
      if ( fields.size() < 2 || !isdigit( fields[ 0 ][ 0 ] ) ) {
        // Skip malformed lines
        continue;
      }
    }
    else {
      // Process lines with valid block:data format
      string block = line.substr( 0, colonPos ); // Block number
      string data = line.substr( colonPos + 1 ); // Rest of the data

      // Split data into fields
      vector<string> fields = split( data, ',' );

      // Extract zip codes (skip 5 fields for each)
      for ( size_t i = 0; i < fields.size(); i += 6 ) {
        if ( !fields[ i ].empty() && isdigit( fields[ i ][ 0 ] ) ) {
          string zipCode = fields[ i ];
          outputFile << zipCode << "," << block << "\n";
        }
      }
    }
  }

  inputFile.close();
  outputFile.close();

  cout << "Data successfully organized and saved to '" << outputFileName << "'.\n";
}

