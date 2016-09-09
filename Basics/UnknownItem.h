//! \file
//! \author Shujian Yang
//!
//! Header file of class UnknownItem

#ifndef UNKNOWN_ITEM_H
#define UNKNOWN_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics{
    //! Unknown item data.
    class UnknownItem : public BtrfsItem {
    public:
        //! Constructor of unknown item.
        UnknownItem(ItemHead* head):BtrfsItem(head) {}
        ~UnknownItem() = default; //!< Destructor

        //! Infomation unavailable yet.
        std::string dataInfo() const override { return "Unknown item"; }
    };
}

#endif

