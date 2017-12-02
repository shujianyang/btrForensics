//! \file
//! \author Shujian Yang
//!
//! Header file of class BtrfsPool.

#ifndef BTRFS_POOL_H
#define BTRFS_POOL_H

#include <map>
#include <vector>
#include <functional>
#include "DeviceRecord.h"
#include "Basics/Basics.h"
#include "Trees/Trees.h"

namespace btrForensics {

    //! Manage devices registered for a btrfs filesystem.
    class BtrfsPool {
    public:
        TSK_IMG_INFO *image; 
        TSK_ENDIAN_ENUM endian; //!< Endianness.

        UUID fsUUID;
        SuperBlock* primarySupblk;
        std::map<uint64_t, DeviceRecord*> deviceTable;


        ChunkTree* chunkTree; //!< The chunk tree.
        FilesystemTree* fsTree; //!< The file system tree.
        FilesystemTree* fsTreeDefault; //!< Default file system tree.
        const BtrfsNode* rootTree; //!< Root node of the root tree.

    public:
        BtrfsPool(TSK_IMG_INFO*, TSK_ENDIAN_ENUM, vector<TSK_OFF_T>);
        ~BtrfsPool();

        uint64_t getDevOffset(const uint64_t devId) const;

        std::vector<uint64_t> getAddrFromChunk(uint64_t logicalAddr,
                const BtrfsKey* key, const ChunkData* chunkData) const;
        uint64_t getTempAddrFromChunk(uint64_t logicalAddr,
                const BtrfsKey* key, const ChunkData* chunkData) const;
        uint64_t readData(char *data, const uint64_t logicalAddr, const BtrfsKey* key,
               const ChunkData* chunkData, const uint64_t size) const;


        void initializeChunkTree();
        void initializeRootTree();
        uint64_t getDefaultFsId();

        bool getPhyAddrFromChunkTree(const LeafNode* leaf,
                uint64_t targetLogAddr, uint64_t& targetPhyAddr) const;
        void navigateNodes(const BtrfsNode* root, std::ostream& os, std::istream& is) const;
        const bool switchFsTrees(std::ostream& os, std::istream& is);

        void treeTraverse(const BtrfsNode* node,
            std::function<void(const LeafNode*)> readOnlyFunc) const;

        bool treeSearch(const BtrfsNode* node,
            std::function<bool(const LeafNode*)> searchFunc) const;

        bool treeSearchById(const BtrfsNode* node, uint64_t targetId,
            std::function<bool(const LeafNode*, uint64_t)> searchFunc) const;
    };
}

#endif

