/** \file
  * Implementation of class RootItem
  */

#include "RootItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    /**
     * Constructor of root item.
     *
     * \param endian The endianess of the array.
     * \param arr Byte array storing root item data.
     * 
     */
    RootItem::RootItem(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :inode(endian, arr), dropProgress(endian, arr + 0xdc)
    {
        int arIndex(InodeItem::SIZE_OF_INODE_ITEM);

        exptGen = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        objIdInThisTree = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        blkNum = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        byteLimit = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        byteUsed = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        lastGenOfSnapshot = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        for(int i=0; i<0x08; i++){
            flags[i] = arr[arIndex++];
        }

        numOfRefs = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        arIndex += BtrfsKey::SIZE_OF_KEY;

        dropLevel = arr[arIndex++];
        rootLevel = arr[arIndex++];
    }


    /**
     * Get block number of the root node.
     *
     */
    const uint64_t RootItem::getBlockNumber() const
    {
        return blkNum;
    }


    /**
     * Overloaded stream operator.
     *
     */
    std::ostream &operator<<(std::ostream &os, const RootItem &root)
    {
        os << "Object id in tree: " << root.objIdInThisTree << '\n';

        os << std::uppercase << std::hex
            << "Block number: " << root.blkNum << '\n';

        os << "Bytes used: " << root.byteUsed << '\n';

        //os << "Number of items: " << header.numOfItems << '\n';
        //os << "Level: " << (int)header.level;

        return os;
    }

}
