/** \file
  * Header file of class RootItem
  */

#ifndef ROOT_ITEM_H
#define ROOT_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics{
    /** Root item data.
     *
     */
    class RootItem{
    public:
        const InodeItem inode;
    private:
        uint64_t exptGen; //0xa0
        uint64_t objIdInThisTree;

        uint64_t blkNum;  //0xb0
        uint64_t byteLimit;

        uint64_t byteUsed; //0xc0
        uint64_t lastGenOfSnapshot;

        uint8_t flags[8]; //0xd0
        uint32_t numOfRefs;
    public:
        const BtrfsKey dropProgress;
    private:
        uint8_t dropLevel;
        uint8_t rootLevel;

    public:
        RootItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        const uint64_t getBlockNumber() const;

        friend std::ostream &operator<<(std::ostream &os, const RootItem &root);
    };
}

#endif

