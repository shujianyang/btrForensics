//! \file
//! \author Shujian Yang
//!
//! Header file of class BtrfsItem

#ifndef BTR_ITEM_H
#define BTR_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "ItemHead.h"

namespace btrForensics{
    //! Btrfs item in a leaf node, this is an abstract class.
    class BtrfsItem {
    public:
        const ItemHead* itemHead ; //!< Item stored in leaf node.
        
        //! Constructor of item
        BtrfsItem(ItemHead* head):itemHead(head) {}
        virtual ~BtrfsItem() { delete itemHead; }

        //! Get id of the item.
        const uint64_t getId() const { return itemHead->key.objId; }
        //! Get type code of the item.
        const ItemType getItemType() const { return itemHead->key.itemType; }
        
        //! Overloaded stream operator.
        friend std::ostream &operator<<(std::ostream &os, const BtrfsItem &item)
        {
            os << "[Item]\n";
            os << *item.itemHead << "\n[Data]\n" << item.dataInfo();
            os << "\n" << std::endl;
            return os;
        }
        

        //! Return infomation about the item data.
        //! Virtual function to be overridden by derived classes.
        virtual std::string dataInfo() const = 0;
    };
}

#endif

