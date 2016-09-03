/**
 * \file
 * \author Shujian Yang
 *
 * Header file of class InodeRef
 */

#ifndef INODE_REF_H
#define INODE_REF_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics{
    /** Inode reference data. */
    class InodeRef : public ItemData {
    private:
        uint64_t indexInDir;
        uint16_t nameSize;
        char *nameInDir;

    public:
        InodeRef(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        std::string getDirName() const;

        std::string info() const override;
    };
}

#endif

