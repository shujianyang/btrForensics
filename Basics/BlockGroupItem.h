//! \file
//! \author Shujian Yang
//!
//! Header file of class BlockGroupItem

#ifndef BLK_GRP_ITEM_H
#define BLK_GRP_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics {
    //! Block group item.
    class BlockGroupItem : public BtrfsItem {
    private:
        uint64_t usedAmount;
        uint64_t chunkTreeId;
        uint64_t flags;

    public:
        BlockGroupItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~BlockGroupItem() = default;

        std::string dataInfo() const override;
    };
}

#endif

