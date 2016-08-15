/**
 * \file
 * \author Shujian Yang
 *
 * Assistant functions defined here.
 *
 */

 #ifndef FUNCTIONS_H
 #define FUNCTIONS_H

#include <iostream>
#include <vector>
#include "Tools.h"

namespace btrForensics{

    void printLeafDir(const LeafNode*, std::vector<uint64_t>&, std::ostream&);

}

#endif
