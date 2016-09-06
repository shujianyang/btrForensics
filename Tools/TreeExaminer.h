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
#include "Structures/Structures.h"

namespace btrForensics {
    class ChunkTree;
    class FilesystemTree;

    //! Examine a tree in btrfs.
    class TreeExaminer {
    public:
        ChunkTree* chunkTree; //!< The chunk tree.
        FilesystemTree* fsTree; //!< The file system tree.
        const BtrfsNode* rootTree; //!< Root node of the root tree.

        TSK_IMG_INFO* image; //!< Image file.
        TSK_ENDIAN_ENUM endian; //!< Endianness.

    public:
        TreeExaminer(TSK_IMG_INFO*, TSK_ENDIAN_ENUM, const SuperBlock*);
        ~TreeExaminer() = default;

        uint64_t getPhysicalAddr(uint64_t logicalAddr) const;

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
