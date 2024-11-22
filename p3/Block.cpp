#include "Block.h"
#include <iostream>
#include <fstream>
#include <sstream>  // For string stream
#include <vector>
#include <map>

using namespace std;

/**
 * @brief Global map of blocks indexed by Relative Block Number (RBN).
 * 
 * This map holds all blocks, with the RBN as the key and the block as the value.
 */
map<int, Block> blocks;

/**
 * @brief Head of the active block list (RBN).
 * 
 * Represents the first block in the logical sequence of active blocks.
 */
int listHeadRBN = -1;

/**
 * @brief Head of the available block list (RBN).
 * 
 * Represents the first block in the list of available (free) blocks.
 */
int availHeadRBN = -1;

/**
 * @brief Creates a block file from an input CSV file.
 * 
 * Reads the input CSV file, splits the data into blocks of a specified size, and writes the blocks to an output file.
 * Each block is identified by its Relative Block Number (RBN) and contains a list of records.
 * 
 * @param inputFile Path to the input CSV file.
 * @param outputFile Path to the output block file.
 * @param BLOCK_SIZE Maximum size of each block in bytes.
 * @return True if the block file was successfully created, false otherwise.
 */
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

/**
 * @brief Parses a block file and populates the global map of blocks.
 * 
 * Reads the block file, extracts block data, and populates the global `blocks` map with the parsed data.
 * 
 * @param blockFile Path to the block file to parse.
 */
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

/**
 * @brief Dumps blocks in physical order based on their RBNs.
 * 
 * Iterates through all blocks in the global map in ascending RBN order and displays their details.
 */
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

/**
 * @brief Dumps blocks in logical order starting from the active list head.
 * 
 * Traverses the logical chain of blocks using the `successorRBN` and displays their details.
 */
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

/**
 * @brief Creates a new block and inserts it into the global map.
 * 
 * @param RBN Relative Block Number of the new block.
 * @param isAvailable Flag indicating whether the block is available (true) or active (false).
 * @param records List of records to store in the block.
 * @param predecessorRBN RBN of the predecessor block in the chain.
 * @param successorRBN RBN of the successor block in the chain.
 */
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

/**
 * @brief Main function to create a block file, parse it, and dump block data.
 * 
 * Executes the following steps:
 * - Creates a block file from an input CSV file.
 * - Parses the generated block file to populate the global `blocks` map.
 * - Dumps the blocks in physical and logical order.
 * 
 * @return int Exit code.
 */
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
