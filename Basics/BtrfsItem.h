/** \file
  * Header file of class BtrfsItem
  */

#ifndef BTR_ITEM_H
#define BTR_ITEM_H

#include "BtrfsKey.h"

namespace btrForensics{

    class BtrfsItem{
    private:
        BtrfsKey key;
        uint32_t dateOffset; //Relative to end of header
        uint32_t dataSize;
    };
}

#endif

