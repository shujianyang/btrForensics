/** \file
  * Header file of class InodeItem
  */

#ifndef INODE_ITEM_H
#define INODE_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>

namespace btrForensics{
    /** Inode item data.
     *
     */
    class InodeItem{
    public:
        const BtrfsKey key;
    private:
        uint64_t generation;
        uint64_t transId;

        uint64_t stSize; //0x10
        uint64_t stBlocks;
        
        uint64_t blockGroup; //0x20
        uint16_t stNlink;
        uint16_t stUid;
        uint16_t stGid;
        uint16_t stMode;
        uint64_t stRdev;

        uint8_t flags[8]; //0x40
        uint64_t sequence;

    public:
        InodeItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

    };
}

#endif

