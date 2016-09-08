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
#include "Basics/Basics.h"
#include "DirContent.h"

namespace btrForensics {
    class TreeExaminer;

    //! Analyze the file system tree in btrfs.
    class FilesystemTree {
    public:
        const BtrfsNode *fileTreeRoot; //!< Root node of the filesystem tree.
        uint64_t rootDirId; //!< Inode number of root directory.

    private:
        const TreeExaminer* examiner;

    public:
        FilesystemTree(const BtrfsNode*, uint64_t rootItemId, const TreeExaminer*);
        //~FilesystemTree() { delete fileTreeRoot; }

        const void listDirItems(std::ostream& os) const;

        DirContent* getDirConent(uint64_t id) const;

        const void explorFiles(std::ostream& os, std::istream& is) const;
    };
}

#endif
