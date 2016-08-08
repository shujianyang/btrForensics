/** \file
  * Header file of class InodeItem
  */

#ifndef INODE_ITEM_H
#define INODE_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>

namespace btrForensics{
    /** Inode item data. */
    class InodeItem{
    private:
        uint64_t generation;
        uint64_t transId;

        uint64_t stSize; //0x10
        uint64_t stBlocks;
        
        uint64_t blockGroup; //0x20
        uint32_t stNlink;
        uint32_t stUid;
        uint32_t stGid;
        uint32_t stMode;
        uint64_t stRdev;

        uint8_t flags[8]; //0x40
        uint64_t sequence;

    public:
        InodeItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        static const int SIZE_OF_INODE_ITEM = 0xa0;
    };
}

#endif

