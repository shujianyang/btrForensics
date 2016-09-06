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
#include "Structures/Structures.h"
#include "TreeExaminer.h"

namespace btrForensics {
    //! Process chunk tree of Btrfs.
    class ChunkTree {
    private:
        const BtrfsNode* chunkRoot;

        const TreeExaminer* examiner;

    public:
        ChunkTree(const SuperBlock* superBlk, const TreeExaminer* treeExaminer);
        
        uint64_t getPhysicalAddr(uint64_t logicalAddr);
    };

}

#endif
