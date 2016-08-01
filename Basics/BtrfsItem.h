/** \file
  * Header file of class BtrfsItem
  */

#ifndef BTR_ITEM_H
#define BTR_ITEM_H

#include <iostream>
#include <tsk/libtsk.h>
#include "BtrfsKey.h"

namespace btrForensics{

    class BtrfsItem{
    private:
        BtrfsKey key;
        uint32_t dataOffset; //Relative to end of header
        uint32_t dataSize;

        //Total bytes: 0x19

    public:
        BtrfsItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        friend std::ostream &operator<<(std::ostream &os, BtrfsItem &item);
    };
}

#endif

