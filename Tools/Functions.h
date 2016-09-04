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

    bool findItem(const LeafNode*, uint64_t, uint8_t, const BtrfsItem*&);

    bool findNewItem(const LeafNode*, uint64_t, uint8_t, vector<BtrfsItem*>&);
}

#endif
