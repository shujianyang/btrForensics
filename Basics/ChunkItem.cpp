//! \file
//! \author Shujian Yang
//!
//! Implementation of class ChunkItem

#include <sstream>
#include "ChunkItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of ChunkItem.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing chunk item data.
    //!
    ChunkItem::ChunkItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head), data(endian, arr)
    {
    }

    //! Return infomation about the item data as string.
    std::string ChunkItem::dataInfo() const
    {
        return data.dataInfo();
    }

}
