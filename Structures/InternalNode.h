/** \file
 * Header file of class BtrfsNode.
 */

#ifndef INTERNAL_NODE_H
#define INTERNAL_NODE_H

#include <vector>
#include "BtrfsNode.h"

using std::vector;

namespace btrForensics{
    /** Internal node in B-tree structure. */
    class InternalNode : public BtrfsNode {
    private:
        vector<KeyPtr> keyPointers;

    public:
        InternalNode(BtrfsHeader *header, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
    };

}

#endif

