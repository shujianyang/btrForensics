/** \file
  * Header file of class BtrfsItem
  */

#ifndef BTR_ITEM_H
#define BTR_ITEM_H

#include <iostream>
#include <tsk/libtsk.h>
#include "BtrfsKey.h"

namespace btrForensics{
    /** Node items, stored right after node header.
     * May contain data.
     */
    class BtrfsItem{
    private:
        const BtrfsKey key;
        uint32_t dataOffset; //Relative to end of header
        uint32_t dataSize;

        //Total bytes: 0x19

    public:
        BtrfsItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        friend std::ostream &operator<<(
            std::ostream &os, const BtrfsItem &item);

        static const int SIZE_OF_ITEM = 0x19; /**< Size of an item in bytes. */
    };
}

#endif

