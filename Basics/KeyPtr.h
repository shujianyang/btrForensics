//! \file
//! \author Shujian Yang
//!
//! Header file of class KeyPtr

#ifndef KEY_PTR_H
#define KEY_PTR_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsKey.h"
//#include "Trees/BtrfsNode.h"

namespace btrForensics{
    class BtrfsNode;

    //! Key pointers stored in internal nodes, stored right after node header.
    class KeyPtr{
    public:
        const BtrfsKey key; //!< Key of the key pointer.
        const BtrfsNode* childNode; //!< Pointer to the child node pointed by this key pointer.
    private:
        uint64_t blkNum;
        uint64_t generation;

        //Total bytes: 0x21

    public:
        KeyPtr(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~KeyPtr();

        const uint64_t getBlkNum() const { return blkNum; }  //!< Return block number.
        const uint32_t getGeneration() const { return generation; }  //!< Return generation.

        friend std::ostream &operator<<(
            std::ostream &os, const KeyPtr &keyPtr);

        static const int SIZE_OF_KEY_PTR = 0x21; //!< Size of a key pointer in bytes.
    };
}

#endif

