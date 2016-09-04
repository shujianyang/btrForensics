//! \file
//! \author Shujian Yang
//!
//! Header file of class RootItem

#ifndef ROOT_ITEM_H
#define ROOT_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsItem.h"
#include "InodeData.h"

namespace btrForensics{
    //! Root item data.
    class RootItem : public BtrfsItem {
    public:
        const InodeData inode; //!< Inode

        uint64_t exptGen; //0xa0
        uint64_t rootObjId;

        uint64_t blkNum;  //0xb0
        uint64_t byteLimit;

        uint64_t byteUsed; //0xc0
        uint64_t lastGenOfSnapshot;

        uint8_t flags[8]; //0xd0
        uint32_t numOfRefs;

        const BtrfsKey dropProgress; //!< Always 0(?)

        uint8_t dropLevel;
        uint8_t rootLevel;

    public:
        RootItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        const uint64_t getBlockNumber() const;
        std::string dataInfo() const override;
        
        friend std::ostream &operator<<(std::ostream &os, const RootItem &root);
    };
}

#endif

