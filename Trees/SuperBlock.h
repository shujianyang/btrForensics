//!
//! \file
//! \author Shujian Yang
//!
//! Header file of SuperBlock.

#ifndef SUP_BLK_H
#define SUP_BLK_H

#include <iostream>
#include <string>
#include "Utility/Utility.h"
#include "Basics/Basics.h"

namespace btrForensics{
    //! Store super block info.
    class SuperBlock{
    public:
        static constexpr int DEV_ITEM_SIZE = 0x62;
        static constexpr int LABEL_SIZE = 0x100;

        uint8_t checksum[0x20]; //0x0
        
        const UUID fsUUID; //0x20

        uint64_t address; //0x30
        uint8_t flags[0x8];

        char magic[0x8]; //0x40
        uint64_t generation;

        uint64_t rootTrRootAddr; //0x50
        uint64_t chunkTrRootAddr;

        uint64_t logTrRootAddr; //0x60
        uint64_t logRootTransid;

        uint64_t totalBytes; //0x70
        uint64_t bytesUsed;
        
        uint64_t rootDirObjectid; //0x80
        uint64_t numDevices;

        uint32_t sectorSize; //0x90
        uint32_t nodeSize;
        uint32_t leafSize;
        uint32_t stripeSize;

        uint32_t sysChunkArrSize; //0xa0
        uint64_t chunkRootGeneration;
        uint64_t compatFlags;
        uint64_t compatRoFlags;
        uint64_t imcompatFlags;

        uint16_t csumType;
        uint8_t rootLevel;
        uint8_t chunkRootLevel;
        uint8_t logRootLevel;

        const DevData devItemData; //0xc9
        uint8_t label[LABEL_SIZE];

        const BtrfsKey chunkKey;
        const ChunkData chunkData;

    public:
        SuperBlock(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~SuperBlock() = default; //!< Destructor

        const uint64_t getChunkPhyAddr() const;
        const uint64_t getRootLogAddr() const;

        const std::string printMagic() const;
        const std::string printSpace() const;
        const std::string printLabel() const;

        static const int SUPBLK_ADDR = 0x10000;  //!< Address of superblock on disk.
        static const int SUPBLK_SIZE = 0xb2b;  //!< Size of superblock on disk.

        friend std::ostream &operator<<(std::ostream &os, SuperBlock &supb);
    };
}

#endif
