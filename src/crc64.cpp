// Header Files
#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include "crc64.h"

// Constans

const uint64_t CRC64_INITIAL = 0xFFFFFFFFFFFFFFFF;
/**
 * @brief Lookup table for CRC-64 checksum calculation.
 * 
 * This table is used to calculate the CRC-64 checksum of a given input data. It is a constant array of 256 uint64_t values.
 * Each value in the array is the result of a CRC-64 calculation on a single byte of input data. The table is used to speed up
 * the calculation of the CRC-64 checksum by allowing the algorithm to process multiple bytes of input data at once.
 * 
 * The CRC-64 algorithm is commonly used for error detection in data transmission and storage systems.
 * 
 * @see https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 */
const uint64_t crc_table[] = {
    0x0000000000000000, 0x01B0000000000000, 0x0360000000000000, 0x02D0000000000000, 
    0x06C0000000000000, 0x0770000000000000, 0x05A0000000000000, 0x0410000000000000, 
    0x0D80000000000000, 0x0C30000000000000, 0x0EE0000000000000, 0x0F50000000000000, 
    0x0B40000000000000, 0x0AF0000000000000, 0x0820000000000000, 0x0990000000000000, 
    0x1B00000000000000, 0x1AB0000000000000, 0x1860000000000000, 0x19D0000000000000, 
    0x1DC0000000000000, 0x1C70000000000000, 0x1EA0000000000000, 0x1F10000000000000, 
    0x1680000000000000, 0x1730000000000000, 0x15E0000000000000, 0x1450000000000000, 
    0x1040000000000000, 0x11F0000000000000, 0x1320000000000000, 0x1290000000000000,
    0x3600000000000000, 0x37B0000000000000, 0x3560000000000000, 0x34D0000000000000,
    0x30C0000000000000, 0x3170000000000000, 0x33A0000000000000, 0x3210000000000000,
    0x3B80000000000000, 0x3A30000000000000, 0x38E0000000000000, 0x3950000000000000,
    0x3D40000000000000, 0x3CF0000000000000, 0x3E20000000000000, 0x3F90000000000000,
    0x2D00000000000000, 0x2CB0000000000000, 0x2E60000000000000, 0x2FD0000000000000,
    0x2BC0000000000000, 0x2A70000000000000, 0x28A0000000000000, 0x2910000000000000,
    0x2080000000000000, 0x2130000000000000, 0x23E0000000000000, 0x2250000000000000,
    0x2640000000000000, 0x27F0000000000000, 0x2520000000000000, 0x2490000000000000,
    0x6C00000000000000, 0x6DB0000000000000, 0x6F60000000000000, 0x6ED0000000000000,
    0x6AC0000000000000, 0x6B70000000000000, 0x69A0000000000000, 0x6810000000000000,
    0x6180000000000000, 0x6030000000000000, 0x62E0000000000000, 0x6350000000000000,
    0x6740000000000000, 0x66F0000000000000, 0x6420000000000000, 0x6590000000000000,
    0x7700000000000000, 0x76B0000000000000, 0x7460000000000000, 0x75D0000000000000,
    0x71C0000000000000, 0x7070000000000000, 0x72A0000000000000, 0x7310000000000000,
    0x7A80000000000000, 0x7B30000000000000, 0x79E0000000000000, 0x7850000000000000,
    0x7C40000000000000, 0x7DF0000000000000, 0x7F20000000000000, 0x7E90000000000000,
    0x5A00000000000000, 0x5BB0000000000000, 0x5960000000000000, 0x58D0000000000000,
    0x5CC0000000000000, 0x5D70000000000000, 0x5FA0000000000000, 0x5E10000000000000,
    0x5780000000000000, 0x5630000000000000, 0x54E0000000000000, 0x5550000000000000,
    0x5140000000000000, 0x50F0000000000000, 0x5220000000000000, 0x5390000000000000,
    0x4100000000000000, 0x40B0000000000000, 0x4260000000000000, 0x43D0000000000000,
    0x47C0000000000000, 0x4670000000000000, 0x44A0000000000000, 0x4510000000000000,
    0x4C80000000000000, 0x4D30000000000000, 0x4FE0000000000000, 0x4E50000000000000,
    0x4A40000000000000, 0x4BF0000000000000, 0x4920000000000000, 0x4890000000000000,
    0xD800000000000000, 0xD9B0000000000000, 0xDB60000000000000, 0xDAD0000000000000,
    0xDEC0000000000000, 0xDF70000000000000, 0xDDA0000000000000, 0xDC10000000000000,
    0xD580000000000000, 0xD430000000000000, 0xD6E0000000000000, 0xD750000000000000,
    0xD340000000000000, 0xD2F0000000000000, 0xD020000000000000, 0xD190000000000000,
    0xC300000000000000, 0xC2B0000000000000, 0xC060000000000000, 0xC1D0000000000000,
    0xC5C0000000000000, 0xC470000000000000, 0xC6A0000000000000, 0xC710000000000000,
    0xCE80000000000000, 0xCF30000000000000, 0xCDE0000000000000, 0xCC50000000000000,
    0xC840000000000000, 0xC9F0000000000000, 0xCB20000000000000, 0xCA90000000000000,
    0xEE00000000000000, 0xEFB0000000000000, 0xED60000000000000, 0xECD0000000000000,
    0xE8C0000000000000, 0xE970000000000000, 0xEBA0000000000000, 0xEA10000000000000,
    0xE380000000000000, 0xE230000000000000, 0xE0E0000000000000, 0xE150000000000000,
    0xE540000000000000, 0xE4F0000000000000, 0xE620000000000000, 0xE790000000000000,
    0xF500000000000000, 0xF4B0000000000000, 0xF660000000000000, 0xF7D0000000000000,
    0xF3C0000000000000, 0xF270000000000000, 0xF0A0000000000000, 0xF110000000000000,
    0xF880000000000000, 0xF930000000000000, 0xFBE0000000000000, 0xFA50000000000000,
    0xFE40000000000000, 0xFFF0000000000000, 0xFD20000000000000, 0xFC90000000000000,
    0xB400000000000000, 0xB5B0000000000000, 0xB760000000000000, 0xB6D0000000000000,
    0xB2C0000000000000, 0xB370000000000000, 0xB1A0000000000000, 0xB010000000000000,
    0xB980000000000000, 0xB830000000000000, 0xBAE0000000000000, 0xBB50000000000000,
    0xBF40000000000000, 0xBEF0000000000000, 0xBC20000000000000, 0xBD90000000000000,
    0xAF00000000000000, 0xAEB0000000000000, 0xAC60000000000000, 0xADD0000000000000,
    0xA9C0000000000000, 0xA870000000000000, 0xAAA0000000000000, 0xAB10000000000000,
    0xA280000000000000, 0xA330000000000000, 0xA1E0000000000000, 0xA050000000000000,
    0xA440000000000000, 0xA5F0000000000000, 0xA720000000000000, 0xA690000000000000,
    0x8200000000000000, 0x83B0000000000000, 0x8160000000000000, 0x80D0000000000000,
    0x84C0000000000000, 0x8570000000000000, 0x87A0000000000000, 0x8610000000000000,
    0x8F80000000000000, 0x8E30000000000000, 0x8CE0000000000000, 0x8D50000000000000,
    0x8940000000000000, 0x88F0000000000000, 0x8A20000000000000, 0x8B90000000000000,
    0x9900000000000000, 0x98B0000000000000, 0x9A60000000000000, 0x9BD0000000000000,
    0x9FC0000000000000, 0x9E70000000000000, 0x9CA0000000000000, 0x9D10000000000000,
    0x9480000000000000, 0x9530000000000000, 0x97E0000000000000, 0x9650000000000000,
    0x9240000000000000, 0x93F0000000000000, 0x9120000000000000, 0x9090000000000000
};

//remake the algorithm so the format is similar to crc32. e.g: 12f90801

// Function Prototypes
uint64_t crc64_update(const char *data, size_t length, uint64_t crc);
void crc64(const char *data, size_t length);

// Function Definitions
uint64_t crc64_update(const char *data, size_t length, uint64_t crc) {
    for (size_t i = 0; i < length; ++i) {
        crc ^= (uint64_t) data[i];
        crc = (crc >> 8) ^ crc_table[crc & 0xFF];
    }
    return crc;
}

/*void crc64(const char *data, size_t length) {
    uint64_t crc = CRC64_INITIAL;
    crc = crc64_update(data, length, crc);
    crc ^= CRC64_INITIAL;
    fprintf(stderr,"64bit CRC: 0x%08X%08X\n", (uint32_t)(crc >> 32), (uint32_t)(crc & 0xFFFFFFFF));
} */

// this keeps returning 0x3063600000000000
// fix this so it returns a proper crc64 checksum
void crc64(const char *data, size_t length) {
    uint64_t crc = CRC64_INITIAL;
    crc = crc64_update(data, length, crc);
    crc ^= CRC64_INITIAL;
    std::cout << "64bit CRC: 0x" << std::hex << crc << std::endl;
}

/*// Function Prototypes
uint64_t crc64_update(const char *data, size_t length, uint64_t crc);
void crc64(const char *data, size_t length);

// Function Definitions
uint64_t crc64_update(const char *data, size_t length, uint64_t crc) {
    for (size_t i = 0; i < length; ++i) {
        crc ^= (uint64_t) data[i];
        crc = (crc >> 8) ^ crc_table[crc & 0xFF];
    }
    return crc;
}*/

/*void crc64(const char *data, size_t length) {
    uint64_t crc = CRC64_INITIAL;
    crc = crc64_update(data, length, crc);
    crc ^= CRC64_INITIAL;
    fprintf(stderr,"64bit CRC: 0x%08X%08X\n", (uint32_t)(crc >> 32), (uint32_t)(crc & 0xFFFFFFFF));
} */

// this keeps returning 0x3063600000000000
// fix this so it returns a proper crc64 checksum
/*
void crc64(const char *data, size_t length) {
    uint64_t crc = CRC64_INITIAL;
    crc = crc64_update(data, length, crc);
    crc ^= CRC64_INITIAL;
    std::cout << "64bit CRC: 0x" << std::hex << crc << std::endl;
}*/



int main(int argc, char **argv) {
    if (argc < 2) {
        // quick help if no arguments
        std::cout << "Usage: crc64 [input data]" << std::endl;
        return 0;
    }
    //help command
    /*if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        std::cout << "Help Usage: crc64 [input data]" << std::endl;
        return 0;
    }*/
    //rewrite this to include -h and --help
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        std::cout << "Help Usage: crc64 [input data]" << std::endl;
        return 0;
    }
    //version command
    if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)) {
        std::cout << "Version: " << VERSION << std::endl;
        return 0;
    }

    //file command
    if (argc == 3 && (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0)) {
        // get the file and open it.
        FILE *file = fopen(argv[2], "r");
        if (file == NULL) {
            std::cerr << "Error: File not found" << std::endl;
            return 1;
        }
        // read the file
        char *buffer = NULL;
        size_t len = 0;
        size_t read;
        while ((read = getline(&buffer, &len, file)) != (size_t)-1)
        {
            crc64(buffer, read);
            return 0;
        }
        
    }
        
    // Getinput data
    std::string data = argv[1];

    // Calculate CRC 
    crc64(data.c_str(), data.size());

    return 0;
}
