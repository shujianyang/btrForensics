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

    public:
        FileTreeAnalyzer(TSK_IMG_INFO*, const LeafNode*, TSK_ENDIAN_ENUM);

        const void listDirItems(std::ostream& os) const;

    };
}

#endif
