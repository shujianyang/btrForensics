//! \file
//! \author Shujian Yang
//!
//! Header file of class DirItem

#ifndef DIR_ITEM_H
#define DIR_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics{
    //! Directory item data.
    class DirItem : public ItemData {
    public:
        const BtrfsKey key; //!< Key of the item.
    private:
        uint64_t transId;
        uint16_t dataSize;
        uint16_t nameSize;
        uint8_t childType;
        char *dirName;
        char *dirData;

    public:
        DirItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        uint8_t getType() { return childType; } //!< Get the type of directory item.
        std::string getDirName() const;

        std::string info() const override;
    };
}

#endif

