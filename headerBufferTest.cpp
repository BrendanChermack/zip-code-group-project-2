#include "HeaderBuffer.h"
#include <iostream>

int main() {
    HeaderBuffer header;

    // Set header fields
    header.setFileStructureType("CSV");
    header.setVersion("1.0");
    header.setHeaderSize(256);
    header.setRecordSizeBytes(4);
    header.setSizeFormat("ASCII");
    header.setIndexFileName("headerBufferTest.csv");
    header.setRecordCount(1000);
    header.setFieldCount(6);
    header.setPrimaryKeyField(0);

    // Add field metadata
    header.addFieldMetadata("zip_code", "string");
    header.addFieldMetadata("city", "string");
    header.addFieldMetadata("state_id", "string");
    header.addFieldMetadata("county", "string");
    header.addFieldMetadata("latitude", "double");
    header.addFieldMetadata("longitude", "double");

    // Write header to file
    if (header.writeHeader("headerBufferTest.csv")) {
        std::cout << "Header written to zipcode_data.txt" << std::endl;
    } else {
        std::cerr << "Failed to write header" << std::endl;
    }

    // Read header from file
    HeaderBuffer readHeader;
    if (readHeader.readHeader("zipcode_data.txt")) {
        std::cout << "Header read successfully:" << std::endl;
        std::cout << "File structure type: " << readHeader.getFileStructureType() << std::endl;
        std::cout << "Version: " << readHeader.getVersion() << std::endl;
        std::cout << "Header size: " << readHeader.getHeaderSize() << std::endl;
        std::cout << "Record size bytes: " << readHeader.getRecordSizeBytes() << std::endl;
        std::cout << "Size format: " << readHeader.getSizeFormat() << std::endl;
        std::cout << "Index file name: " << readHeader.getIndexFileName() << std::endl;
        std::cout << "Record count: " << readHeader.getRecordCount() << std::endl;
        std::cout << "Field count: " << readHeader.getFieldCount() << std::endl;
        std::cout << "Primary key field: " << readHeader.getPrimaryKeyField() << std::endl;

        const auto& fields = readHeader.getFields();
        std::cout << "Field metadata:" << std::endl;
        for (const auto& field : fields) {
            std::cout << "  Name: " << field.name << ", Type: " << field.typeSchema << std::endl;
        }
    } else {
        std::cerr << "Failed to read header" << std::endl;
    }

    return 0;
}