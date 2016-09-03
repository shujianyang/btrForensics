//! \file
//! \author Shujian Yang
//!
//! Header file of class ItemGroup.

#ifndef ITEM_GROUP_H
#define ITEM_GROUP_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics/Basics.h"

namespace btrForensics{
    //! An item group consists of an item and its data stored in a leaf node.
    class ItemGroup {
    public:
        const BtrfsItem *item; //!< Item stored in leaf node.
        const ItemData *data; //!< Corresponding item data.

        //! Constructor of item group.
        ItemGroup(BtrfsItem *itemIn, ItemData *dataIn)
            :item(itemIn), data(dataIn) {}
        ~ItemGroup() { delete item; delete data; }

        //! Get id of the item.
        const uint64_t getId() const { return item->key.objId; }
        //! Get type code of the item.
        const uint8_t getItemType() const { return item->key.getItemType(); }

        //! Overloaded stream operator.
        friend std::ostream &operator<<(std::ostream &os, const ItemGroup &itmGrp)
        {
            os << *itmGrp.item << '\n' << *itmGrp.data;
            return os;
        }
    };

}

#endif

