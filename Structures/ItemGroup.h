/** \file
 * Header file of class ItemGroup.
 */

#ifndef ITEM_GROUP_H
#define ITEM_GROUP_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics/Basics.h"

namespace btrForensics{
    /**
     * An item group consists of an item and its data
     * stored in a leaf node.
     */
    class ItemGroup {
    public:
        const BtrfsItem *item;
        const ItemData *data;

        ItemGroup(BtrfsItem *itemIn, ItemData *dataIn)
            :item(itemIn), data(dataIn) {}
        ~ItemGroup() { delete item; delete data; }

        friend std::ostream &operator<<(std::ostream &os, const ItemGroup &itmGrp)
        {
            os << *itmGrp.item << '\n' << *itmGrp.data;
            return os;
        }
    };

}

#endif

