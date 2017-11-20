//! \file
//! \author Shujian Yang
//!
//! Header file of class BtrfsExaminer.

#ifndef BTRFS_EXAMINER_H
#define BTRFS_EXAMINER_H

#include <vector>
#include "DeviceRecord.h"
#include "Basics/Basics.h"
#include "Trees/Trees.h"

namespace btrForensics {

    //! Manage devices registered for a btrfs filesystem.
    class BtrfsExaminer {
    public:
        UUID fsUUID;
        std::vector<SuperBlock*> supblkVec;

    public:
        BtrfsExaminer(uint64_t, uint64_t, UUID);
        ~DeviceRocord() = default; //!< Destructor
    };
}

#endif

