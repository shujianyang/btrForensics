/** \file
  * Header file of class DirIndex
  */

#ifndef DIR_INDEX_H
#define DIR_INDEX_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics{
    /** Directory index data. */
    class DirIndex: public ItemData {
    public:
        const BtrfsKey key; /**< Key of the index. */
    private:
        uint64_t transId;
        uint16_t dataSize;
        uint16_t nameSize;
        uint8_t childType;
        char *dirName;
        char *dirData;

    public:
        DirIndex(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        std::string getDirName() const;

        std::string info() const override;
    };
}

#endif

