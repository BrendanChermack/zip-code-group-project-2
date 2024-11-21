#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <unordered_map>

// Define ZipCodeRecord structure
struct ZipCodeRecord {
    std::string zip_code;
    std::string city;
    std::string state_id;
    double latitude;
    double longitude;
};

class Buffer {
public:
    // Method to read a CSV file and store records in the buffer
    bool read_csv(const std::string& us_postal_code.csv, size_t records_per_block = 100);

    // Method to get all Zip Code records, grouped by state
    std::map<std::string, std::vector<ZipCodeRecord>> get_state_zip_codes() const;

    // Method to get the block records
    std::unordered_map<size_t, std::unordered_map<std::string, ZipCodeRecord>> get_blocks() const;

    // Method to dump block data for debugging purposes
    void dump_blocks() const;

private:
    // Vector to store ZipCodeRecord entries
    std::vector<ZipCodeRecord> records;

    // HashMap of blocks, where each block has Zip Codes as the key and ZipCodeRecord as the value
    std::unordered_map<size_t, std::unordered_map<std::string, ZipCodeRecord>> blocks;

    // Method to parse a line from CSV into ZipCodeRecord
    ZipCodeRecord parse_csv_line(const std::string& line) const;

    // Method to add a ZipCodeRecord to a specific block
    void add_record(size_t block_number, const ZipCodeRecord& record);
};

#endif // BUFFER_H
