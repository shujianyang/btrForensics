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
     * \param arr Byte array storing header data.
     * 
     */
    BtrfsHeader::BtrfsHeader(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :fsUUID(endian, arr + 0x20), chunkTrUUID(endian, arr + 0x40)
    {
        int arIndex(0);
        for(int i=0; i<0x20; i++){
            checksum[i] = arr[arIndex++];
        }

        arIndex += 0x10; //fsUUID, initialized ahead.

        address = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        for(int i=0; i<0x08; i++){
            flags[i] = arr[arIndex++];
        }

        arIndex += 0x10; //chunkTrUUID, initialized ahead.

        generation = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        treeId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        numOfItems = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        level = arr[arIndex++];
    }


    /**
     * Get number of items stored in this node.
     *
     */
    const uint32_t BtrfsHeader::getNumOfItems() const
    {
        return numOfItems;
    }


    /**
     * Overloaded stream operator.
     *
     */
    std::ostream &operator<<(std::ostream &os, const BtrfsHeader &header)
    {
        os << "FS UUID: " << header.fsUUID.encode() << '\n';

        os << std::uppercase << std::hex
            << "Node address: " << header.address << '\n';

        os << "Chunk tree UUID: " << header.chunkTrUUID.encode() << '\n';

        os << "Generation: " << header.generation << '\n';
        os << "Tree id: " << header.treeId << '\n';
        os << "Number of items: " << header.numOfItems << '\n';
        os << "Level: " << (int)header.level;

        return os;
    }

}
