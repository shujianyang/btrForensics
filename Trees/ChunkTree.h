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
    class BtrfsPool;
    class LeafNode;

    //! Process chunk tree of Btrfs.
    class ChunkTree {
    public:
        const BtrfsNode* chunkRoot; //!< Root of chunk tree.
    private:
        BtrfsPool* btrPool;

    public:
        ChunkTree(BtrfsPool *pool);
        ~ChunkTree();
        
        bool findChunkItem(const LeafNode* leaf, uint64_t targetLogAddr,
                const ChunkItem* &foundChunk) const;
        const ChunkItem* getChunkItem(uint64_t logicalAddr) const;
    };

}

#endif
