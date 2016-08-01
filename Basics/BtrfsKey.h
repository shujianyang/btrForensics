/** \file
  * Header file of class BtrfsKey
  */

#ifndef BTR_KEY_H
#define BTR_KEY_H

#include <iostream>
#include <tsk/libtsk.h>
#include <string>

namespace btrForensics{

    class BtrfsKey{
    private:
        uint64_t objId;
        uint8_t itemType;
        uint64_t offset;

        //Total bytes: 0x11

    public:
        BtrfsKey(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        friend std::ostream &operator<<(std::ostream &os, BtrfsKey &key);

        uint8_t getItemType();
        std::string getItemTypeStr();
    };
}

#endif
