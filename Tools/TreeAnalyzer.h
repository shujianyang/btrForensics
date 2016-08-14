/** \file
 * Header file of class TreeAnalyzer.
 */

#ifndef TREE_ANA_H
#define TREE_ANA_H

#include <iostream>
#include <vector>
#include <tsk/libtsk.h>
#include "Tools.h"

namespace btrForensics {
    /** Analyze a tree in btrfs. */
    class TreeAnalyzer {
    public:
        const LeafNode *root; /**< Root node of the tree. */
        const BtrfsNode *fileTreeRoot; /**< Root node of the filesystem tree. */
        TSK_IMG_INFO *image; /**< Image file. */
        TSK_ENDIAN_ENUM endian; /**< Endianness. */

    public:
        TreeAnalyzer(TSK_IMG_INFO*, const LeafNode*, TSK_ENDIAN_ENUM);

        const void navigateNodes(std::ostream&, std::istream&) const;

        const void recursiveListDir(const BtrfsNode*,
                std::ostream&, std::vector<uint64_t>&) const;
        const void listDirItems(std::ostream&) const;

        void leafRecursion(const BtrfsNode*, void(*process)(const LeafNode*));
        void printLeafDir(const LeafNode*);
    };
}

#endif
