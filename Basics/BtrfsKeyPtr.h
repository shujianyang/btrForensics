/** \file
  * Header file of class BtrfsKeyPtr
  */

#ifndef BTR_KEY_PTR_H
#define BTR_KEY_PTR_H

#include "BtrfsKey.h"

namespace btrForensics{

    class BtrfsKeyPtr{
    private:
        BtrfsKey key;
        uint64_t blockNum;
        uint64_t generation;
    };
}

#endif

