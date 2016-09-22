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
    //! \param headerEnd Physical address of leaf header ending.
    //! \param addr Offset from leaf header endint to this item.
    //!
    ItemHead::ItemHead(TSK_ENDIAN_ENUM endian, uint8_t arr[], uint64_t headerEnd, uint64_t offset)
        :key(endian, arr), itemPhyAddr(headerEnd + offset)
    {
        int arIndex(BtrfsKey::SIZE_OF_KEY); //Key initialized already.
        dataOffset = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        dataSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        dataPhyAddr = headerEnd + dataOffset;
    }


    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream &os, const ItemHead &itemHead)
    {
        os << "Item physical address: " << itemHead.itemPhyAddr << '\n';
        os << "Data physical address: " << itemHead.dataPhyAddr << "\n\n";
        os << itemHead.key << '\n';

        return os;
    }

}
