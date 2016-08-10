/** \file
 * Header file of class TreeAnalyzer.
 */

#ifndef TREE_ANA_H
#define TREE_ANA_H

#include <iostream>
#include <map>
#include <tsk/libtsk.h>
#include "Tools.h"

namespace btrForensics {
    /** Analyze a tree in btrfs. */
    class TreeAnalyzer {
    public:
        const BtrfsNode *root; /**< Root node of the tree. */
        TSK_IMG_INFO *image; /**< Image file. */
        TSK_ENDIAN_ENUM endian; /**< Endianness. */

    public:
        TreeAnalyzer(TSK_IMG_INFO*, BtrfsNode*, TSK_ENDIAN_ENUM);

        void recursiveListDir(const BtrfsNode*, std::ostream&) const;
        void listDirItems(std::ostream&) const;
    };
}

#endif
