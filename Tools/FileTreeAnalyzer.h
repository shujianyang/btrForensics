//! \file
//! \author Shujian Yang
//!
//! Header file of class FileTreeAnalyzer.

#ifndef FILE_TREE_ANA_H
#define FILE_TREE_ANA_H

#include <iostream>
#include <vector>
#include <functional>
#include <tsk/libtsk.h>
#include "TreeAnalyzer.h"

namespace btrForensics {
    //! Analyze the file system tree in btrfs.
    class FileTreeAnalyzer : public TreeAnalyzer {
    public:
        const BtrfsNode *fileTreeRoot; //!< Root node of the filesystem tree.
        uint64_t rootDirId; //!< Inode number of root directory.

    public:
        FileTreeAnalyzer(TSK_IMG_INFO*, const BtrfsNode*, TSK_ENDIAN_ENUM);
        ~FileTreeAnalyzer() { delete fileTreeRoot; }

        const void listDirItems(std::ostream& os) const;

        DirContent* getDirConent(uint64_t id) const;
        DirContent* getRootDir() const;

        const void explorFiles(std::ostream& os, std::istream& is) const;
    };
}

#endif
