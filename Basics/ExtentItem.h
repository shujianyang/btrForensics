//! \file
//! \author Shujian Yang
//!
//! Header file of class ExtentItem

#ifndef EXTENT_ITEM_H
#define EXTENT_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics {
    //! Extent item.
    class ExtentItem : public BtrfsItem {
    private:
        uint64_t refCount;
        uint64_t generation;
        uint64_t flags;
        BtrfsKey key;
        uint8_t level;

    public:
        ExtentItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~ExtentItem() = default;

        std::string dataInfo() const override;
    };
}

#endif

