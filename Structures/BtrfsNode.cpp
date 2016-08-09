/** \file
 * Implementation of class BtrfsNode.
 */

#include "BtrfsNode.h"

namespace btrForensics{
    /**
     * Constructor of btrfs node.
     *
     * \param header Pointer to header of a node.
     * \param endian The endianess of the array.
     * \param arr Byte array storing node data.
     *
     */
    BtrfsNode::BtrfsNode(BtrfsHeader *header)
        :nodeHeader(header)
    {
    }
}
