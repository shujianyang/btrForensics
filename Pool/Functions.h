//! \file
//! \author Shujian Yang
//!
//! Assistant functions defined here.

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include "Pool.h"

namespace btrForensics{

    void printLeafDir(const LeafNode*, std::ostream&);

    bool searchForItem(const LeafNode*, uint64_t, ItemType, const BtrfsItem*&);

    bool filterItems(const LeafNode*, uint64_t, ItemType, vector<const BtrfsItem*>&);

    void filterItems(const LeafNode*, ItemType, vector<const BtrfsItem*>&);

    std::ostream &operator<<(std::ostream& os, const DirItemType& type);
}

#endif
