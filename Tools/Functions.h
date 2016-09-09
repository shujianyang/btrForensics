//! \file
//! \author Shujian Yang
//!
//! Assistant functions defined here.

 #ifndef FUNCTIONS_H
 #define FUNCTIONS_H

#include <iostream>
#include <vector>
#include "Tools.h"

namespace btrForensics{

    void printLeafDir(const LeafNode*, std::ostream&);

    bool searchForItem(const LeafNode*, uint64_t, ItemType, const BtrfsItem*&);

    bool filterItems(const LeafNode*, uint64_t, ItemType, vector<const BtrfsItem*>&);

    void filterItems(const LeafNode*, ItemType, vector<const BtrfsItem*>&);

    bool getPhyAddr(const LeafNode* leaf, uint64_t targetLogAddr, uint64_t& targetPhyAddr);

    uint64_t getChunkAddr(uint64_t logicalAddr,
                const BtrfsKey* key, const ChunkData* chunkData);
}

#endif
