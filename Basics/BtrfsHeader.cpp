/** \file
  * Implementation of class BtrfsHeader
  */

#include "BtrfsHeader.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    /**
     * Constructor of Btrfs header.
     *
     * \param endian The endianess of the array.
     * \param arr Byte array storing super block data.
     * 
     */
    BtrfsHeader::BtrfsHeader(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :fsUUID(endian, arr + 0x20), chunkTrUUID(endian, arr + 0x40)
    {
        int arIndex(0);
        for(int i=0; i<0x20; i++){
            checksum[i] = arr[arIndex++];
        }

        arIndex += 0x20; //fsUUID, initialized ahead.

        address = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        for(int i=0; i<0x08; i++){
            flags[i] = arr[arIndex++];
        }

        arIndex += 0x20; //chunkTrUUID, initialized ahead.

        generation = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        treeId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        numOfItems = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        level = arr[arIndex++];
    }


    /**
     * Overloaded stream operator.
     *
     */
    std::ostream &operator<<(std::ostream &os, BtrfsHeader &header)
    {
        os << "FS UUID: " << header.fsUUID.encode() << '\n';

        os << std::uppercase << std::hex
            << "Node address: " << header.address << '\n';

        os << "Chunk tree UUID: " << header.chunkTrUUID.encode() << '\n';

        os << "Generation: " << header.generation << '\n';
        os << "Tree id: " << header.treeId << '\n';
        os << "Number of items: " << header.numOfItems << '\n';
        os << "Level: " << header.level;

        return os;
    }

}