//! \file
//! \author Shujian Yang
//!
//! Implementation of class BtrfsNode.

#include "BtrfsNode.h"

namespace btrForensics{
    //! Constructor of btrfs node.
    //!
    //! \param header Pointer to header of a node.
    //!
    BtrfsNode::BtrfsNode(const BtrfsHeader *header)
        :nodeHeader(header) {}
}
