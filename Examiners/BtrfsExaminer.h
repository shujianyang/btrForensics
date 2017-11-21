//! \file
//! \author Shujian Yang
//!
//! Header file of class BtrfsExaminer.

#ifndef BTRFS_EXAMINER_H
#define BTRFS_EXAMINER_H

#include <map>
#include <vector>
#include "DeviceRecord.h"
#include "Basics/Basics.h"
#include "Trees/Trees.h"

namespace btrForensics {

    //! Manage devices registered for a btrfs filesystem.
    class BtrfsExaminer {
    public:
        UUID fsUUID;
        std::map<uint64_t, DeviceRecord*> deviceTable;
        //std::vector<SuperBlock*> supblkVec;

    public:
        BtrfsExaminer(TSK_IMG_INFO *img, vector<TSK_OFF_T> partOffsets);
        ~BtrfsExaminer() = default; //!< Destructor
    };
}

#endif

