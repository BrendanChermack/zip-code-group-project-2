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

    // Helper function to write length-indicated field
    auto writeField = [&file](const std::string& value) {
        std::string lengthStr = std::to_string(value.length());
        if (lengthStr.length() < 2) lengthStr = "0" + lengthStr;
        file << lengthStr << value;
    };

    // Write main header fields
    writeField(fileStructureType); file << ",";
    writeField(version); file << ",";
    writeField(std::to_string(headerRecordSize)); file << ",";
    writeField(std::to_string(recordSizeBytes)); file << ",";
    writeField(sizeFormatType); file << ",";
    writeField(indexFileName); file << ",";
    writeField(std::to_string(recordCount)); file << ",";
    writeField(std::to_string(fieldCount)); file << ",";
    writeField(std::to_string(primaryKeyField));
    file << "\n";

    // Write field metadata
    for (const auto& field : fields) {
        writeField(field.name); file << ",";
        writeField(field.typeSchema); file << "\n";
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
        
        // Helper function to read length-indicated field
        auto readField = [](std::stringstream& ss) -> std::string {
            std::string lenStr;
            lenStr.resize(2);
            ss.read(&lenStr[0], 2);
            
            if (!std::isdigit(lenStr[0]) || !std::isdigit(lenStr[1])) {
                throw std::runtime_error("Invalid length indicator");
            }
            
            int length = std::stoi(lenStr);
            std::string value;
            value.resize(length);
            ss.read(&value[0], length);
            
            if (ss.peek() == ',') ss.ignore();
            return value;
        };

        try {
            fileStructureType = readField(ss);
            version = readField(ss);
            headerRecordSize = std::stoi(readField(ss));
            recordSizeBytes = std::stoi(readField(ss));
            sizeFormatType = readField(ss);
            indexFileName = readField(ss);
            recordCount = std::stoi(readField(ss));
            fieldCount = std::stoi(readField(ss));
            primaryKeyField = std::stoi(readField(ss));

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