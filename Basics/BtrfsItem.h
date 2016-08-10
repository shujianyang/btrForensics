/** \file
  * Header file of class BtrfsItem
  */

#ifndef BTR_ITEM_H
#define BTR_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsKey.h"

namespace btrForensics{
    /** Node items, stored right after node header. */
    class BtrfsItem {
    public:
        const BtrfsKey key; /**< Key of the item. */
    private:
        uint32_t dataOffset; /**< Relative to end of header. */
        uint32_t dataSize;

        //Total bytes: 0x19

    public:
        BtrfsItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        /**
         * Return offset of data linked to this item,
         * relative to end of header.
         */
        const uint32_t getDataOffset() const { return dataOffset; } 

        /** Return size of data linked to this item. */
        const uint32_t getDataSize() const { return dataSize; }

        friend std::ostream &operator<<(
            std::ostream &os, const BtrfsItem &item);

        static const int SIZE_OF_ITEM = 0x19; /**< Size of an item in bytes. */
    };
}

#endif

