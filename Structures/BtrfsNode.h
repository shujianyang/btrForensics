/**
 * \file
 * \author Shujian Yang
 *
 * Header file of class BtrfsNode.
 */

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Utility/Utility.h"
#include "Basics/Basics.h"

namespace btrForensics{
    /** Represent a node in B-tree structure. */
    class BtrfsNode {
    public:
        const BtrfsHeader *nodeHeader; /**< Header of a node. */

        BtrfsNode(const BtrfsHeader *header);
        ~BtrfsNode() { delete nodeHeader; }

        /**
         * Return infomation about the node.
         * Virtual function to be overridden by derived classes.
         */
        virtual const std::string info() const = 0;
    };

}

#endif

