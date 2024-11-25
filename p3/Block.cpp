#include "Block.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "HeaderRecord.h"

using namespace std;

/**
 * @brief Global map of blocks indexed by Relative Block Number (RBN).
 * 
 * This map stores all blocks, where the key is the RBN, and the value is the block object.
 */
map<int, Block> blocks;

/**
 * @brief Head of the active block list (RBN).
 * 
 * Stores the RBN of the first block in the active (logical) sequence.
 */
int listHeadRBN = -1;

/**
 * @brief Head of the available block list (RBN).
 * 
 * Stores the RBN of the first block in the available (free) list.
 */
int availHeadRBN = -1;

/**
 * @brief Creates a block file from an input CSV file.
 * 
 * This function reads an input CSV file, divides its data into fixed-size blocks, 
 * and writes those blocks into a new output file.
 * 
 * @param inputFile Path to the input CSV file.
 * @param outputFile Path to the output block file.
 * @param BLOCK_SIZE Maximum size of each block in bytes.
 * @return True if the file was successfully created, false otherwise.
 */
bool createBlockFile(const std::string& inputFile, const std::string& outputFile, size_t BLOCK_SIZE) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (!inFile.is_open() || !outFile.is_open()) {
        cerr << "Error: Could not open input or output file: " << inputFile << " | " << outputFile << endl;
        return false;
    }

    HeaderRecord header;
    
    // Set basic header information
    header.setFileStructureType("blocked_sequence_set");
    header.setVersion("1.0");
    header.setBlockSize(512);  // Default block size
    header.setMinBlockCapacity(0.5);  // 50% minimum capacity
    header.setIndexFileName("headerTest.idx");
    header.setIndexSchema("key:string,rbn:int");
    
    // Set primary key field (zip_code is field 0)
    header.setPrimaryKeyField(0);

    // First write the header
    if (!header.writeHeader(outFile)) {
        std::cerr << "Failed to write header to output file" << std::endl;
        return false;
    }

    size_t blockNumber = 1;               ///< Current block number being written
    size_t currentBlockSize = 0;          ///< Current size of the block in bytes
    vector<string> blockRecords;          ///< Records for the current block

    string line;
    getline(inFile, line); // Skip header
    while (getline(inFile, line)) {
        size_t lineSize = line.size() + 1; // Include newline character
        if (currentBlockSize + lineSize > BLOCK_SIZE) {
            // Write the current block to the output file
            outFile << blockNumber << ":";
            for (size_t i = 0; i < blockRecords.size(); i++) {
                outFile << blockRecords[i];
                if (i < blockRecords.size() - 1) outFile << ",";
            }
            outFile << "\n";

            blockRecords.clear();
            currentBlockSize = 0;
            blockNumber++;
        }

        blockRecords.push_back(line);
        currentBlockSize += lineSize;
    }

    // Write the last block if there are remaining records
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
 * This function reads a block file, splits its content into blocks, 
 * and populates the `blocks` map with their respective details.
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
        int RBN = stoi(line.substr(0, colonPos));  ///< Extracted RBN of the block
        string recordsPart = line.substr(colonPos + 1);

        vector<string> records;                   ///< Records in the block
        stringstream recordStream(recordsPart);
        string record;
        while (getline(recordStream, record, ',')) {
            records.push_back(record);
        }

        // Create a block using the parsed data
        createBlock(RBN, false, records, -1, -1);
    }

    inFile.close();
}

/**
 * @brief Dumps all blocks in physical order.
 * 
 * This function iterates through all blocks stored in the global `blocks` map 
 * and prints their details in ascending order of their RBNs.
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
 * @brief Dumps all blocks in logical order starting from the active list head.
 * 
 * This function follows the logical chain of blocks using their successor links 
 * and prints the details of each block in sequence.
 */
void dumpLogicalOrder() {
    cout << "Dumping Blocks by Logical Order:\n";
    int currentRBN = listHeadRBN;  ///< Start from the logical list head
    while (currentRBN != -1) {
        const Block& block = blocks[currentRBN];
        cout << "RBN: " << currentRBN << " ";
        for (const string& record : block.records) {
            cout << record << " ";
        }
        cout << "\n";
        currentRBN = block.successorRBN;  ///< Move to the next block in the chain
    }
}

/**
 * @brief Creates a new block and inserts it into the global map.
 * 
 * This function initializes a new block with the provided details and inserts 
 * it into the `blocks` map. It also updates the global head pointers for the 
 * active and available block lists as needed.
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

    // Update the global head pointers
    if (!isAvailable && listHeadRBN == -1) {
        listHeadRBN = RBN;
    }
    if (isAvailable && availHeadRBN == -1) {
        availHeadRBN = RBN;
    }
}
