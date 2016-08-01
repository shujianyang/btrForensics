/** \file
 * Implementation of class BtrfsItem
 */

#include "BtrfsItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    /**
     * Constructor of Btrfs header.
     *
     * \param endian The endianess of the array.
     * \param arr Byte array storing super block data.
     *
     */
    BtrfsItem::BtrfsItem(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :key(endian, arr)
    {
        int arIndex(0);
        dataOffset = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        dataSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;
    }


    /**
     * Overloaded stream operator.
     *
     */
    std::ostream &operator<<(std::ostream &os, BtrfsItem &item)
    {
        os << item.key;
        os << "Data offset: " << item.dataOffset << '\n';
        os << "Data size: " << item.dataSize << '\n';

        return os;
    }

}
