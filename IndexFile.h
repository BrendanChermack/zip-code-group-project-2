#ifndef INDEX_FILE_H
#define INDEX_FILE_H

#include <iostream>
#include <vector>
#include <string>

class IndexFile {
    public:
        bool createIndexFile( const std::string& csvFile, const std::string& outputFile );
};

#endif