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

    bool searchForItem(const LeafNode*, uint64_t, uint8_t, const BtrfsItem* &);

}

#endif
