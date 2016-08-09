/** \file
  * Header file of class UnknownItem
  */

#ifndef UNKNOWN_ITEM_H
#define UNKNOWN_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics{
    /** Unknown item data. */
    class UnknownItem : public ItemData {
    public:
        std::string info() const override { return "Unknown item"; }
    };
}

#endif

