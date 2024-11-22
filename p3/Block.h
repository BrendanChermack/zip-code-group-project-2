/**
 * @file Block.h
 * @brief Declaration of the Block structure and related global variables and functions for managing a blocked sequence set.
 * @author Thomas Hoerger
 * @date 11/21/2024
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>
#include <map>

/**
 * @brief Represents a block in the blocked sequence set.
 */
struct Block {
    int RBN;                           ///< Relative Block Number (unique identifier for the block)
    bool isAvailable;                  ///< Flag indicating whether the block is available
    std::vector<std::string> records;  ///< Records stored in the block
    int predecessorRBN;                ///< RBN of the predecessor block in the chain
    int successorRBN;                  ///< RBN of the successor block in the chain
};

/** 
 * @brief Global map of blocks indexed by Relative Block Number (RBN).
 */
extern std::map<int, Block> blocks;

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
 * This function iterates over all blocks in ascending order of their RBNs and prints their details.
 * Available blocks are marked explicitly.
 */
void dumpPhysicalOrder();

/**
 * @brief Dumps blocks in logical order starting from the active list head.
 * 
 * This function follows the logical chain of blocks using their successor links and prints details of each block.
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
void createBlock(int RBN, bool isAvailable, const std::vector<std::string>& records, int predecessorRBN, int successorRBN);

#endif // BLOCK_H
