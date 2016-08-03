/** \file
  * Header file of class RootItem
  */

#ifndef ROOT_ITEM_H
#define ROOT_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsKey.h"

namespace btrForensics{
    /** Root item data.
     *
     */
    class DirItem{
    public:
        const BtrfsKey key;
    private:
        uint64_t transId;
        uint16_t dataSize;
        uint16_t nameSize;
        uint8_t childType;
        char *dirName;
        char *dirData;

    public:
        DirItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        std::string getDirName();
    };
}

#endif

