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

namespace btrForensics {
    //! Directory item data.
    class DirItem : public BtrfsItem {
    public:
        const BtrfsKey targetKey; //!< Key of the item.
        DirItemType type; //!< Type of directory item.
    private:
        uint64_t transId;
        uint16_t dataSize;
        uint16_t nameSize;
        uint8_t childType;
        char *dirName;
        char *dirData;

    public:
        DirItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~DirItem();

        //! Get inode number of target this item points to.
        uint64_t getTargetInode() { return targetKey.objId; }  

        //! Get item type of target this item points to.
        ItemType getTargetType() { return targetKey.itemType; }  

        std::string getDirName() const;

        std::string dataInfo() const override;
    };
}

#endif

