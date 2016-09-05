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

    bool searchItem(const LeafNode*, uint64_t, ItemType, const BtrfsItem*&);

    bool searchMultiItems(const LeafNode*, uint64_t, ItemType, vector<BtrfsItem*>&);
}

#endif
