/**
 * @file Block.h
 * @brief Header file for the blocked sequence set operations and data structures.
 * 
 * This file contains the declarations for managing blocked sequence sets, including
 * data structures and functions for physical and logical order dumps.
 * 
 * @author Thomas Hoerger
 * @date 11/21/2024
 */

#ifndef BLOCKED_SEQUENCE_SET_H
#define BLOCKED_SEQUENCE_SET_H

#include <vector>
#include <unordered_map>
#include <string>

bool createBlockFile( const string inputFile, const string outputFile );

 /**
 * @struct Block
 * @brief Represents a single block in the blocked sequence set.
 * 
 * A block can either be part of the active list or the available list.
 * It contains metadata such as predecessor and successor links and a list of records.
 */
struct Block {
    int RBN;                            ///< Relative Block Number
    int predecessorRBN;                 ///< RBN of the predecessor block in the chain
    int successorRBN;                   ///< RBN of the successor block in the chain
    std::vector<std::string> records;   ///< List of records in the block
    bool isAvailable;                   ///< Flag indicating if the block is available
};

/** 
 * @brief Global map of blocks indexed by Relative Block Number (RBN). 
 */
extern std::unordered_map<int, Block> blocks;

/** 
 * @brief Head of the active block list (RBN). 
 */
extern int listHeadRBN;

/** 
 * @brief Head of the available block list (RBN). 
 */
extern int availHeadRBN;

/**
 * @brief Dumps blocks in physical order based on their RBNs.
 * 
 * This function iterates over all blocks in ascending order of their RBNs
 * and prints their details, including predecessor and successor links.
 */
void dumpPhysicalOrder();

/**
 * @brief Dumps blocks in logical order starting from the active list head.
 * 
 * This function traverses the logical chain of blocks using their successor
 * links and prints their details, including predecessor and successor links.
 */
void dumpLogicalOrder();

/**
 * @brief Creates a new block and inserts it into the global map.
 * 
 * @param RBN Relative Block Number of the new block.
 * @param isAvailable Flag indicating whether the block is available (true) or active (false).
 * @param records List of records to store in the block.
 * @param predecessorRBN RBN of the predecessor block in the chain.
 * @param successorRBN RBN of the successor block in the chain.
 */
void createBlock(int RBN, bool isAvailable, const std::vector<std::string>& records,
                 int predecessorRBN, int successorRBN);

#endif // BLOCKED_SEQUENCE_SET_H
