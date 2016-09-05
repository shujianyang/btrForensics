//! \file
//! \author Shujian Yang
//!
//! Implementation of class RootItem

#include <sstream>
#include "RootItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of root item.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing root item data.
    //!
    RootItem::RootItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head), inode(endian, arr), dropProgress(endian, arr + 0xdc)
    {
        int arIndex(InodeData::SIZE_OF_INODE_DATA);

        exptGen = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        rootObjId = read64Bit(endian, arr + arIndex);
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


    //! Get block number of the root node.
    const uint64_t RootItem::getBlockNumber() const
    {
        return blkNum;
    }


    //! Return infomation about the item data as string.
    std::string RootItem::dataInfo() const
    {
        std::ostringstream oss;
        oss << "Object id of root directory: " << rootObjId << '\n';

        oss << std::uppercase << std::hex
            << "Block number: " << blkNum << '\n';

        oss << "Bytes used: " << byteUsed << '\n';
        return oss.str();
    }

}
