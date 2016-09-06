//! \file
//! \author Shujian Yang
//!
//! Header file of class FilesystemTree.

#ifndef FILE_TREE_H
#define FILE_TREE_H

#include <iostream>
#include <vector>
#include <functional>
#include <tsk/libtsk.h>
#include "TreeExaminer.h"

namespace btrForensics {
    //! Analyze the file system tree in btrfs.
    class FilesystemTree {
    public:
        const BtrfsNode *fileTreeRoot; //!< Root node of the filesystem tree.
        uint64_t rootDirId; //!< Inode number of root directory.

        const TreeExaminer* examiner;

    public:
        FilesystemTree(const BtrfsNode*, const TreeExaminer*);
        ~FilesystemTree() { delete fileTreeRoot; }

        const void listDirItems(std::ostream& os) const;

        DirContent* getDirConent(uint64_t id) const;
        DirContent* getRootDir() const;

        const void explorFiles(std::ostream& os, std::istream& is) const;
    };
}

#endif
