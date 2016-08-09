/** \file
 * Header file of class LeafNode.
 */

#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include <vector>
#include <string>
#include <tsk/libtsk.h>
#include "Structures.h"

using std::vector;

namespace btrForensics{
    /** Leaf node in B-tree structure. */
    class LeafNode : public BtrfsNode {
    private:
        vector<ItemGroup*> itemGroups;

    public:
        LeafNode(TSK_IMG_INFO*, BtrfsHeader*, TSK_ENDIAN_ENUM, uint64_t);

        const std::string info() const;
    };
}

#endif

