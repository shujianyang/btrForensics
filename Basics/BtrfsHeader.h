/** \file
  * Header file of class BtrfsHeader
  */
  
#ifndef BTRFS_HEADER_H
#define BTRFS_HEADER_H

#include "Utility/Uuid.h"

namespace btrForensics{

    class BtrfsHeader{
    private:
        uint8_t checksum[0x20]; //0x0

        UUID fsUUID; //0x20

        uint64_t address; //0x30
        uint8_t flags[0x8];

        UUID chunkTrUUID; //0x40

        uint64_t generation; //0x50
        uint64_t treeId;

        uint32_t numOfItems; //0x60
        uint8_t level;
    };

}

#endif
