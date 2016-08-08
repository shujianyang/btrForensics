/** \file
 * Header file of class LeafNode.
 */

#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include <vector>
#include "BtrfsNode.h"

using std::vector;

namespace BtrfsForensics{
    /** Leaf node in B-tree structure. */
    class LeafNode : public BtrfsNode{
    private:
        vector<BtrfsItem> items;

    public:
        LeafNode(BtrfsHeader *header, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
    };

}

#endif

