//! \file
//! \author Shujian Yang
//!
//! Implementation of class ItemHead

#include "ItemHead.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of Btrfs header.
    //!
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing item head data.
    //!
    ItemHead::ItemHead(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :key(endian, arr)
    {
        int arIndex(BtrfsKey::SIZE_OF_KEY); //Key initialized already.
        dataOffset = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        dataSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;
    }


    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream &os, const ItemHead &itemHead)
    {
        os << itemHead.key;

        return os;
    }

}
