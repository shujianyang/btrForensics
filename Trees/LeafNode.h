//! \file
//! \author Shujian Yang
//!
//! Header file of class LeafNode.

#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include <vector>
#include <string>
#include <tsk/libtsk.h>
#include "Trees.h"

using std::vector;

namespace btrForensics{
    //! Leaf node in B-tree structure.
    class LeafNode : public BtrfsNode {
    public:
        vector<const BtrfsItem*> itemList; //!< Stores items and their data.

    public:
        LeafNode(TSK_IMG_INFO*, const BtrfsHeader*, TSK_ENDIAN_ENUM, uint64_t);
        ~LeafNode();

        const std::string info() const override;
    };
}

#endif

