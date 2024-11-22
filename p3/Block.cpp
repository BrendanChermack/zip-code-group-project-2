#include "Block.h"
#include <iostream>
#include <iomanip>
#include <map>  

using namespace std;

/** 
 * Global map of blocks indexed by Relative Block Number (RBN).
 * This map is used to store all blocks, with the key being the RBN and the value being the block data.
 */
map<int, Block> blocks;

/** 
 * Head of the active block list (RBN).
 * Represents the first block in the logical (active) sequence of blocks.
 */
int listHeadRBN = -1;

/** 
 * Head of the available block list (RBN).
 * Represents the first block in the list of available (free) blocks.
 */
int availHeadRBN = -1;

/**
 * Dumps blocks in physical order based on their RBNs.
 * Iterates through all blocks in the global map in ascending order of their RBNs and displays their details.
 */
void dumpPhysicalOrder() {
    // Print headers for the physical dump
    cout << "Dumping Blocks by Physical Order:\n";
    cout << "List Head: " << listHeadRBN << endl;
    cout << "Avail Head: " << availHeadRBN << endl;

    // Iterate over all blocks in the map, which is naturally sorted by RBN
    for (const auto& [RBN, block] : blocks) {
        cout << "RBN: " << RBN << " "; // Print the RBN of the block
        if (block.isAvailable) {
            cout << "*available* "; // Mark the block as available if applicable
        } else {
            // Print all records stored in the block
            for (const string& record : block.records) {
                cout << record << " ";
            }
        }
        // Print the predecessor and successor RBNs for the block
        cout << "Predecessor: " << block.predecessorRBN 
             << " Successor: " << block.successorRBN << endl;
    }
}

/**
 * Dumps blocks in logical order starting from the active list head.
 * Traverses the logical chain of blocks using the successor RBNs and displays their details.
 */
void dumpLogicalOrder() {
    // Print headers for the logical dump
    cout << "Dumping Blocks by Logical Order:\n";
    cout << "List Head: " << listHeadRBN << endl;
    cout << "Avail Head: " << availHeadRBN << endl;

    int currentRBN = listHeadRBN; // Start from the logical list head
    while (currentRBN != -1) { // Continue until the end of the chain is reached
        const Block& block = blocks[currentRBN]; // Retrieve the current block
        cout << "RBN: " << currentRBN << " "; // Print the RBN
        for (const string& record : block.records) {
            cout << record << " "; // Print the records stored in the block
        }
        // Print the predecessor and successor RBNs for the block
        cout << "Predecessor: " << block.predecessorRBN 
             << " Successor: " << block.successorRBN << endl;
        currentRBN = block.successorRBN; // Move to the next block in the chain
    }
}

/**
 * Creates a new block and inserts it into the global map.
 * 
 * @param RBN Relative Block Number of the new block.
 * @param isAvailable Flag indicating whether the block is available (true) or active (false).
 * @param records List of records to store in the block.
 * @param predecessorRBN RBN of the predecessor block in the chain.
 * @param successorRBN RBN of the successor block in the chain.
 */
void createBlock(int RBN, bool isAvailable, const vector<string>& records, int predecessorRBN, int successorRBN) {
    // Create and populate a new Block object
    Block block;
    block.RBN = RBN; // Assign the RBN
    block.isAvailable = isAvailable; // Set availability status
    block.records = records; // Store the provided records
    block.predecessorRBN = predecessorRBN; // Set the predecessor RBN
    block.successorRBN = successorRBN; // Set the successor RBN

    // Insert the new block into the global map
    blocks[RBN] = block;

    // Update the head pointers if this is the first block in its list
    if (!isAvailable && listHeadRBN == -1) {
        listHeadRBN = RBN;
    }
    if (isAvailable && availHeadRBN == -1) {
        availHeadRBN = RBN;
    }
}

/**
 * Main function to test the blocked sequence set implementation.
 * Creates a series of blocks and performs both physical and logical order dumps.
 * Demonstrates the functionality of the blocked sequence set.
 * 
 * @return int Exit code.
 */
int main() {
    // Simulate block creation with sample data
    createBlock(1, false, {"keya", "keyb", "keyc"}, -1, 2); // Create an active block
    createBlock(2, false, {"keyd", "keye"}, 1, 3); // Create another active block
    createBlock(3, false, {"keyf"}, 2, -1); // Create the last active block
    createBlock(4, true, {}, -1, -1); // Create an available (free) block

    // Perform physical order dump
    cout << "----- Physical Order Dump -----" << endl;
    dumpPhysicalOrder();
    cout << endl;

    // Perform logical order dump
    cout << "----- Logical Order Dump -----" << endl;
    dumpLogicalOrder();

    return 0; // Return success code
}
