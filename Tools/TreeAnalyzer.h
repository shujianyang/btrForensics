//! \file
//! \author Shujian Yang
//!
//! Header file of class TreeAnalyzer.

#ifndef TREE_ANA_H
#define TREE_ANA_H

#include <iostream>
#include <vector>
#include <functional>
#include <tsk/libtsk.h>
#include "Basics/Basics.h"
#include "Structures/Structures.h"

namespace btrForensics {
    //! Analyze a tree in btrfs.
    class TreeAnalyzer {
    public:
        const LeafNode *root; //!< Root node of the tree.
        TSK_IMG_INFO *image; //!< Image file.
        TSK_ENDIAN_ENUM endian; //!< Endianness.

    public:
        TreeAnalyzer(TSK_IMG_INFO*, const LeafNode*, TSK_ENDIAN_ENUM);

        const void navigateNodes(std::ostream& os, std::istream& is) const;

        void leafTraverse(const BtrfsNode* node,
            std::function<void(const LeafNode*)> readOnlyFunc) const;

        bool leafSearch(const BtrfsNode* node,
            std::function<bool(const LeafNode*)> searchFunc) const;

        bool leafSearchById(const BtrfsNode* node, uint64_t targetId,
            std::function<bool(const LeafNode*, uint64_t)> searchFunc) const;
    };
}

#endif
