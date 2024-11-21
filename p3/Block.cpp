/**
 * @file Block.cpp
 * @brief Implementation of blocked sequence set operations, including physical and logical order dumps.
 * @author Thomas Hoerger
 * @date 11/21/2024
 */

#include "Block.h"
#include <iostream>
#include <iomanip>
#include <algorithm>  // For std::sort

using namespace std;

/** 
 * @brief Global map of blocks indexed by Relative Block Number (RBN). 
 */
unordered_map<int, Block> blocks;

/** 
 * @brief Head of the active block list (RBN). 
 */
int listHeadRBN = -1;

/** 
 * @brief Head of the available block list (RBN). 
 */
int availHeadRBN = -1;

/**
 * @brief Dumps blocks in physical order based on their RBNs.
 * 
 * This function iterates over all blocks in ascending order of their RBNs and prints their details.
 * Available blocks are marked explicitly.
 */
void dumpPhysicalOrder() {
    cout << "Dumping Blocks by Physical Order:\n";
    cout << "List Head: " << listHeadRBN << endl;
    cout << "Avail Head: " << availHeadRBN << endl;

    // Store RBNs in sorted order for consistent output
    vector<int> rbns;
    for (const auto& [RBN, _] : blocks) {
        rbns.push_back(RBN);
    }

    // Sort the RBNs for physical order (ascending RBN)
    sort(rbns.begin(), rbns.end());

    // Dump blocks in the sorted order
    for (int RBN : rbns) {
        const Block& block = blocks[RBN];
        cout << "RBN: " << RBN << " ";
        if (block.isAvailable) {
            cout << "*available* ";
        } else {
            for (const string& record : block.records) {
                cout << record << " ";
            }
        }
        cout << "Predecessor: " << block.predecessorRBN 
             << " Successor: " << block.successorRBN << endl;
    }
}

/**
 * @brief Dumps blocks in logical order starting from the active list head.
 * 
 * This function follows the logical chain of blocks using their successor links and prints details of each block.
 */
void dumpLogicalOrder() {
    cout << "Dumping Blocks by Logical Order:\n";
    cout << "List Head: " << listHeadRBN << endl;
    cout << "Avail Head: " << availHeadRBN << endl;

    int currentRBN = listHeadRBN;
    while (currentRBN != -1) {
        const Block& block = blocks[currentRBN];
        cout << "RBN: " << currentRBN << " ";
        for (const string& record : block.records) {
            cout << record << " ";
        }
        cout << "Predecessor: " << block.predecessorRBN 
             << " Successor: " << block.successorRBN << endl;
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
 * @brief Main function to test the blocked sequence set implementation.
 * 
 * Creates a series of blocks and performs both physical and logical order dumps.
 * 
 * @return int Exit code.
 */
int main() {
    // Simulate block creation
    createBlock(1, false, {"keya", "keyb", "keyc"}, -1, 2);
    createBlock(2, false, {"keyd", "keye"}, 1, 3);
    createBlock(3, false, {"keyf"}, 2, -1);
    createBlock(4, true, {}, -1, -1); // Available block

    // Perform dumps
    cout << "----- Physical Order Dump -----" << endl;
    dumpPhysicalOrder();
    cout << endl;

    cout << "----- Logical Order Dump -----" << endl;
    dumpLogicalOrder();

    return 0;
}
