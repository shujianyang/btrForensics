//! \file
//! \author Shujian Yang
//!
//! Implementation of class BlockGroupItem

#include <sstream>
#include "BlockGroupItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of block group item.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing block group item data.
    //!
    BlockGroupItem::BlockGroupItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head)
    {
        int arIndex(0); //Key initialized already.
        usedAmount = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        chunkTreeId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        flags = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
    }


    //! Return infomation about the item data as string.
    std::string BlockGroupItem::dataInfo() const
    {
        std::ostringstream oss;
        oss << std::dec;
        oss << "Used amount: " << usedAmount << '\n';
        return oss.str();
    }
}

