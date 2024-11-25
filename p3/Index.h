#ifndef INDEX_H
#define INDEX_H
#include <vector>
#include <string>

using namespace std;

class Index {
public:
  void processBlockData( const string& inputFileName, const string& outputFileName );
  std::vector<std::string> split( const std::string& line, char delimiter );
};

#endif 