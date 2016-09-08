//! \file
//! \author Shujian Yang
//!
//! Header file of class RootRef

#ifndef DIR_REF_ITEM_H
#define DIR_REF_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics {
    //! Can represent both root_ref and root_backref items.
    class RootRef : public BtrfsItem {
    public:
    private:
        uint64_t dirId;
        uint64_t index;
        uint16_t nameSize;
        char *dirName;

    public:
        RootRef(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~RootRef();

        //! Get directory id that contains the subtree.
        uint64_t getDirId() { return dirId; }  

        //! Get index number.
        uint64_t getIndex() { return index; }  

        std::string getDirName() const;

        std::string dataInfo() const override;
    };
}

#endif

