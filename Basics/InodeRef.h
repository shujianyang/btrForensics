//! \file
//! \author Shujian Yang
//!
//! Header file of class InodeRef

#ifndef INODE_REF_H
#define INODE_REF_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsItem.h"

namespace btrForensics{
    //! Inode reference data.
    class InodeRef : public BtrfsItem {
    private:
        uint64_t indexInDir;
        uint16_t nameSize;
        char *nameInDir;

    public:
        InodeRef(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~InodeRef();

        std::string getDirName() const;

        std::string dataInfo() const override;
    };
}

#endif

