/** \file
 * Implementation of class BtrfsNode.
 */

#include "BtrfsNode.h"

namespace btrForensics{
    /**
     * Constructor of btrfs node.
     *
     * \param header Pointer to header of a node.
     *
     */
    BtrfsNode::BtrfsNode(BtrfsHeader *header)
        :nodeHeader(header) {}
}
