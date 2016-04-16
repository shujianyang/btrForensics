/** \file
  * Header file of SuperBlock.
  */

#ifndef SPR_BLK_H
#define SPR_BLK_H

#include <iostream>
#include <string>
#include "Guid.h"

class SuperBlock{
private:
    uint8_t checksum[0x20];
    GUID fsUUID;
    uint64_t address;
    uint8_t flags[0x8];
    char magic[0x8];
    uint64_t generation;
    uint64_t rootTrRoot;
    uint64_t chunkTrRoot;
    uint64_t logTrRoot;

public:
    SuperBlock(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

    std::string printMagic();

    static const int SIZE_OF_SPR_BLK = 0x68;

    friend std::ostream &operator<<(std::ostream &os, SuperBlock &supb);
};

#endif
