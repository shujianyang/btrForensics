//! \file
//! \author Shujian Yang
//!
//! Header file of class TreeExaminer.

#ifndef TREE_EXAMINER_H
#define TREE_EXAMINER_H

#include <iostream>
#include <vector>
#include <functional>
#include <tsk/libtsk.h>
#include "Basics/Basics.h"
#include "Trees/Trees.h"

namespace btrForensics {
    //! Examine a tree in btrfs.
    class TreeExaminer {
    public:
        ChunkTree* chunkTree; //!< The chunk tree.
        FilesystemTree* fsTree; //!< The file system tree.
        FilesystemTree* fsTreeDefault; //!< Default file system tree.
        const BtrfsNode* rootTree; //!< Root node of the root tree.

        TSK_IMG_INFO* image; //!< Image file.
        TSK_ENDIAN_ENUM endian; //!< Endianness.

    public:
        TreeExaminer(TSK_IMG_INFO*, TSK_ENDIAN_ENUM, const SuperBlock*);
        ~TreeExaminer();

        uint64_t getPhysicalAddr(uint64_t logicalAddr) const;

        const void navigateNodes(const BtrfsNode* root, std::ostream& os, std::istream& is) const;
        const void switchFsTrees(std::ostream& os, std::istream& is);

        void treeTraverse(const BtrfsNode* node,
            std::function<void(const LeafNode*)> readOnlyFunc) const;

        bool treeSearch(const BtrfsNode* node,
            std::function<bool(const LeafNode*)> searchFunc) const;

        bool treeSearchById(const BtrfsNode* node, uint64_t targetId,
            std::function<bool(const LeafNode*, uint64_t)> searchFunc) const;
    };
}

#endif
