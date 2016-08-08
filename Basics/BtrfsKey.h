/** \file
  * Header file of class BtrfsKey
  */

#ifndef BTR_KEY_H
#define BTR_KEY_H

#include <iostream>
#include <tsk/libtsk.h>
#include <string>

namespace btrForensics{
    /** Key of an item */
    class BtrfsKey{
    private:
        uint64_t objId;
        uint8_t itemType; /**< For detailed list, please read Btrfs Wiki. */
        uint64_t offset;

        //Total bytes: 0x11

    public:
        BtrfsKey(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        friend std::ostream &operator<<(
            std::ostream &os, const BtrfsKey &key);

        const uint8_t getItemType() const;
        const std::string getItemTypeStr() const;

        static const int SIZE_OF_KEY = 0x11;
    };
}

#endif
