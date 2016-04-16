/** \file
  * Header file of SuperBlock.
  */

#ifndef SPR_BLK_H
#define SPR_BLK_H

#include <string>
#include "Guid.h"

class SuperBlock{
private:
    uint8_t checksum[32];
    GUID fsUUID;
    uint8_t address[8];
    uint8_t flags[8];
    char magic[8];

public:
    SuperBlock(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

    static const int SIZE_OF_SPR_BLK = 0x48;
};

#endif
