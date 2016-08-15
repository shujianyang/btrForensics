/**
 * \file
 * \author Shujian Yang
 *
 * Header file of class BtrfsNode.
 */

#ifndef INTERNAL_NODE_H
#define INTERNAL_NODE_H

#include <vector>
#include <tsk/libtsk.h>
#include "BtrfsNode.h"

using std::vector;

namespace btrForensics{
    /** Internal node in B-tree structure. */
    class InternalNode : public BtrfsNode {
    public:
        vector<KeyPtr> keyPointers; /**< Key pointers to other nodes. */

    public:
        InternalNode(TSK_IMG_INFO*, const BtrfsHeader*, TSK_ENDIAN_ENUM, uint64_t);

        const std::string info() const override;
    };

}

#endif

