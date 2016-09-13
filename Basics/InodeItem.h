//! \file
//! \author Shujian Yang
//!
//! Header file of class InodeItem

#ifndef INODE_ITEM_H
#define INODE_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsItem.h"
#include "InodeData.h"

namespace btrForensics{
    //! Inode item data.
    class InodeItem : public BtrfsItem {
    private:
        InodeData data;

    public:
        InodeItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~InodeItem() = default; //!< Destructor

        std::string printTime() const;

        //! Get size of the file.
        uint64_t getSize() const { return data.getSize(); }

        std::string dataInfo() const override;
    };
}

#endif

