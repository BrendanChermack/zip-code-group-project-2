#include "Block.h"
#include <iostream>
#include <fstream>
#include <sstream>  // For string stream
#include <vector>
#include <map>

using namespace std;

/** 
 * Global map of blocks indexed by Relative Block Number (RBN).
 */
map<int, Block> blocks;

/** 
 * Head of the active block list (RBN).
 */
int listHeadRBN = -1;

/** 
 * Head of the available block list (RBN).
 */
int availHeadRBN = -1;

bool createBlockFile(const std::string& inputFile, const std::string& outputFile, size_t BLOCK_SIZE) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (!inFile.is_open() || !outFile.is_open()) {
        cerr << "Error: Could not open input or output file: " << inputFile << " | " << outputFile << endl;
        return false;
    }

    size_t blockNumber = 1;
    size_t currentBlockSize = 0;
    vector<string> blockRecords;

    string line;
    getline(inFile, line); // Skip header
    while (getline(inFile, line)) {
        size_t lineSize = line.size() + 1; // Include newline character
        if (currentBlockSize + lineSize > BLOCK_SIZE) {
            // Write the current block
            outFile << blockNumber << ":";
            for (size_t i = 0; i < blockRecords.size(); i++) {
                outFile << blockRecords[i];
                if (i < blockRecords.size() - 1) outFile << ",";
            }
            outFile << "\n";

            // Start a new block
            blockRecords.clear();
            currentBlockSize = 0;
            blockNumber++;
        }

        blockRecords.push_back(line);
        currentBlockSize += lineSize;
    }

    // Write the last block
    if (!blockRecords.empty()) {
        outFile << blockNumber << ":";
        for (size_t i = 0; i < blockRecords.size(); i++) {
            outFile << blockRecords[i];
            if (i < blockRecords.size() - 1) outFile << ",";
        }
        outFile << "\n";
    }

    inFile.close();
    outFile.close();

    return true;
}

void parseBlockFile(const string& blockFile) {
    ifstream inFile(blockFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open block file: " << blockFile << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        size_t colonPos = line.find(':');
        int RBN = stoi(line.substr(0, colonPos));
        string recordsPart = line.substr(colonPos + 1);

        vector<string> records;
        stringstream recordStream(recordsPart);
        string record;
        while (getline(recordStream, record, ',')) {
            records.push_back(record);
        }

        createBlock(RBN, false, records, -1, -1);
    }

    inFile.close();
}

void dumpPhysicalOrder() {
    cout << "Dumping Blocks by Physical Order:\n";
    for (const auto& [RBN, block] : blocks) {
        cout << "RBN: " << RBN << " ";
        for (const string& record : block.records) {
            cout << record << " ";
        }
        cout << "\n";
    }
}

void dumpLogicalOrder() {
    cout << "Dumping Blocks by Logical Order:\n";
    int currentRBN = listHeadRBN;
    while (currentRBN != -1) {
        const Block& block = blocks[currentRBN];
        cout << "RBN: " << currentRBN << " ";
        for (const string& record : block.records) {
            cout << record << " ";
        }
        cout << "\n";
        currentRBN = block.successorRBN;
    }
}

void createBlock(int RBN, bool isAvailable, const vector<string>& records, int predecessorRBN, int successorRBN) {
    Block block;
    block.RBN = RBN;
    block.isAvailable = isAvailable;
    block.records = records;
    block.predecessorRBN = predecessorRBN;
    block.successorRBN = successorRBN;

    blocks[RBN] = block;

    if (!isAvailable && listHeadRBN == -1) {
        listHeadRBN = RBN;
    }
    if (isAvailable && availHeadRBN == -1) {
        availHeadRBN = RBN;
    }
}

int main() {
    string inputFile = "us_postal_codes.csv";
    string outputFile = "blocks.txt";

    if (createBlockFile(inputFile, outputFile)) {
        cout << "Block file created successfully.\n";
    } else {
        cerr << "Failed to create block file.\n";
        return 1;
    }

    parseBlockFile(outputFile);

    cout << "----- Physical Order Dump -----\n";
    dumpPhysicalOrder();

    cout << "\n----- Logical Order Dump -----\n";
    dumpLogicalOrder();

    return 0;
}
