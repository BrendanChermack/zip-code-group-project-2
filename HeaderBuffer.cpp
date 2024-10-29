#include "CSVProcessing.h"
#include "buffer.h"
#include "CSVLengthIndicated.h"
#include "HeaderBuffer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

HeaderBuffer::HeaderBuffer() 
    : headerRecordSize(0)
    , recordSizeBytes(0)
    , recordCount(0)
    , fieldCount(0)
    , primaryKeyField(0) {
}

bool HeaderBuffer::writeHeader(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return false;
    }

    // Write all header fields with length indicators
    file << "02" << fileStructureType << ","
         << "02" << version << ","
         << "02" << std::to_string(headerRecordSize) << ","
         << "02" << std::to_string(recordSizeBytes) << ","
         << "02" << sizeFormatType << ","
         << "02" << indexFileName << ","
         << "02" << std::to_string(recordCount) << ","
         << "02" << std::to_string(fieldCount) << ","
         << "02" << std::to_string(primaryKeyField) << std::endl;

    // Write field metadata
    for (const auto& field : fields) {
        file << "02" << field.name << ","
             << "02" << field.typeSchema << std::endl;
    }

    file.close();
    return true;
}

bool HeaderBuffer::readHeader(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for reading: " << filename << std::endl;
        return false;
    }

    std::string line;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string lengthStr, value;

        // Helper lambda to read a length-indicated field
        auto readField = [](std::stringstream& ss) -> std::string {
            std::string len(2, '\0');
            ss.read(&len[0], 2);  // Read 2-digit length
            if (len.empty() || !std::isdigit(len[0]) || !std::isdigit(len[1])) {
                throw std::runtime_error("Invalid length indicator");
            }
            int length = std::stoi(len);
            std::string val(length, '\0');
            ss.read(&val[0], length);
            if (ss.peek() == ',') {
                ss.get();  // Skip comma
            }
            return val;
        };

        try {
            // Read main header fields
            fileStructureType = readField(ss);
            version = readField(ss);
            headerRecordSize = std::stoi(readField(ss));
            recordSizeBytes = std::stoi(readField(ss));
            sizeFormatType = readField(ss);
            indexFileName = readField(ss);
            recordCount = std::stoi(readField(ss));
            fieldCount = std::stoi(readField(ss));
            primaryKeyField = std::stoi(readField(ss));

            // Read field metadata
            fields.clear();
            for (int i = 0; i < fieldCount && std::getline(file, line); i++) {
                std::stringstream fieldSS(line);
                FieldMetadata metadata;
                metadata.name = readField(fieldSS);
                metadata.typeSchema = readField(fieldSS);
                fields.push_back(metadata);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing header: " << e.what() << std::endl;
            return false;
        }
    }

    file.close();
    return true;
}