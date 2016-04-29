/** \file
  * Header file of class BtrfsKey
  */

#ifndef BTR_KEY_H
#define BTR_KEY_H

#include "../Utility.h"

class BtrfsKey{
private:
    uint64_t objId;
    uint8_t itemType;
    uint64_t offset;

public:
    BtrfsKey(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
};

#endif
