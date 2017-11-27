//! \file
//! \author Shujian Yang
//!
//! Header file of class ChunkTree

#ifndef CHUNK_TREE_H
#define CHUNK_TREE_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Basics/Basics.h"

namespace btrForensics {
    class BtrfsExaminer;

    //! Process chunk tree of Btrfs.
    class ChunkTree {
    public:
        const BtrfsNode* chunkRoot; //!< Root of chunk tree.
    private:
        BtrfsExaminer* btrfs;
        //const TreeExaminer* examiner;

    public:
        ChunkTree(BtrfsExaminer *examiner);
        ~ChunkTree();
        
        //int64_t agetPhysicalAddr(uint64_t logicalAddr);
    };

}

#endif
