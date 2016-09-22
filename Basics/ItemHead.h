//! \file
//! \author Shujian Yang
//!
//! Header file of class ItemHead

#ifndef ITEM_HAED_H
#define ITEM_HAED_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsKey.h"

namespace btrForensics{
    //! Node item's head, stored right after node header.
    class ItemHead {
    public:
        const BtrfsKey key; //!< Key of the item.
    private:
        uint32_t dataOffset; //!< Relative to end of header.
        uint32_t dataSize;

        //Total bytes: 0x19

        uint64_t itemPhyAddr; //!< Physical address of item head.
        uint64_t dataPhyAddr; //!< Physical address of item data.
    public:
        ItemHead(TSK_ENDIAN_ENUM endian, uint8_t arr[], uint64_t headerEnd, uint64_t offset);
        ~ItemHead() = default; //!< Destructor

        //! Return offset of data linked to this item,
        //! relative to end of header.
        const uint32_t getDataOffset() const { return dataOffset; } 

        //! Return size of data linked to this item.
        const uint32_t getDataSize() const { return dataSize; }

        friend std::ostream &operator<<(
            std::ostream &os, const ItemHead &itemHead);

        static const int SIZE_OF_ITEM_HEAD = 0x19; //!< Size of an item head in bytes.
    };
}

#endif

