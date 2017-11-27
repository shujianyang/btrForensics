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
        TSK_IMG_INFO *image; 
        TSK_ENDIAN_ENUM endian; //!< Endianness.

        UUID fsUUID;
        SuperBlock* primarySupblk;
        std::map<uint64_t, DeviceRecord*> deviceTable;


        ChunkTree* chunkTree; //!< The chunk tree.

    public:
        BtrfsExaminer(TSK_IMG_INFO*, TSK_ENDIAN_ENUM, vector<TSK_OFF_T>);
        ~BtrfsExaminer();

        uint64_t getDevOffset(const uint64_t devId);

        std::vector<uint64_t> getAddrFromChunk(uint64_t logicalAddr,
                const BtrfsKey* key, const ChunkData* chunkData);

        uint64_t getTempAddrFromChunk(uint64_t logicalAddr,
                const BtrfsKey* key, const ChunkData* chunkData);

        uint64_t readData(char *data, const uint64_t logicalAddr, const BtrfsKey* key,
               const ChunkData* chunkData, const uint64_t size);


        void initializeChunkTree();
    };
}

#endif

