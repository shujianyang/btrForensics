//! \file
//! \author Shujian Yang
//!
//! Implementation of class ExtentItem

#include <sstream>
#include "ExtentItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of extent item.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing extent item data.
    //!
    ExtentItem::ExtentItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head), key(endian, arr+0x18)
    {
        int arIndex(0); //Key initialized already.
        refCount = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        generation = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        flags = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        arIndex += BtrfsKey::SIZE_OF_KEY;

        level = arr[arIndex++];
    }


    //! Return infomation about the item data as string.
    std::string ExtentItem::dataInfo() const
    {
        std::ostringstream oss;
        oss << std::dec;
        oss << "Reference count: " << refCount << '\n';
        oss << "Generation: " << generation << '\n';
        oss << "Flags: " << flags << '\n';
        oss << key;
        oss << (int)level << "\n";
        return oss.str();
    }
}

