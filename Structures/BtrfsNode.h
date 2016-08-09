/** \file
 * Header file of class BtrfsNode.
 */

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "Utility/Utility.h"
#include "Basics/Basics.h"

namespace BtrfsForensics{
    /** Represent a node in B-tree structure. */
    class BtrfsNode{
    public:
        const BtrfsHeader *nodeHeader;

        BtrfsNode(BtrfsHeader *header);
    };

}

#endif

