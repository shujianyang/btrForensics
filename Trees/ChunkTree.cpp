//! \file
//! \author Shujian Yang
//!
//! Header file of class ChunkTree

#include <iostream>
#include "ChunkTree.h"
#include "Pool/Functions.h"

namespace btrForensics {
    //! Constructor of tree analyzer.
    //!
    //! \param pool Pointer to a Btrfs pool.
    //!
    ChunkTree::ChunkTree(BtrfsPool *pool)
            :btrPool(pool)
    {
        const SuperBlock *supBlk = btrPool->primarySupblk;

        char *diskArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        //std::cout << std::hex << supBlk->chunkTrRootAddr << std::endl;
        uint64_t chunkTreePhyAddr = btrPool->readChunkData(diskArr, supBlk->chunkTrRootAddr,
                &(supBlk->chunkKey), &(supBlk->chunkData), BtrfsHeader::SIZE_OF_HEADER);

        BtrfsHeader *chunkHeader = new BtrfsHeader(pool->endian, (uint8_t*)diskArr);
        delete [] diskArr;

        //std::cout << *chunkHeader << std::endl;
        uint64_t itemListAddr = chunkTreePhyAddr + BtrfsHeader::SIZE_OF_HEADER;

        const BtrfsNode* chunkTree;
        if(chunkHeader->isLeafNode())
            chunkRoot = new LeafNode(pool->image, chunkHeader,
                                pool->endian, itemListAddr);
        else
            chunkRoot = new InternalNode(pool->image, chunkHeader,
                                TSK_LIT_ENDIAN, itemListAddr);
    }


    //!< Destructor
    ChunkTree::~ChunkTree()
    {
        if(chunkRoot != nullptr)
            delete chunkRoot;
    }


    //! Find chunk item where the logical address is matched.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param targetLogAddr Logical address to convert.
    //! \param foundChunk Found chunk item
    //!
    //! \return Return true when the chunk is found.
    //!
    bool ChunkTree::findChunkItem(const LeafNode* leaf, uint64_t targetLogAddr,
           const ChunkItem* &foundChunk) const
    {
        const BtrfsItem* target(nullptr);

        for(auto item : leaf->itemList) {
            //The item must be a chunk item.
            if(item->getItemType() != ItemType::CHUNK_ITEM)
                continue;
            //Chunk logical address should be just smaller than or equal to
            //target logical address.
            //In other words, find the chunk with logcial address that is the
            //largest one but smaller or equal to target logical address.
            if(item->itemHead->key.offset <= targetLogAddr)
                target = item;
            else
                break;
        }

        if(target == nullptr)
            return false;

       foundChunk = static_cast<const ChunkItem*>(target);
        
        return true;
    }


    //! Convert logical address to physical address.
    //!
    //! \param logicalAddr 64-bit logial address.
    //! \return 64-bit physical address.
    //!
    const ChunkItem* ChunkTree::getChunkItem(uint64_t logicalAddr) const
    {
        const ChunkItem* chunk(nullptr);
        btrPool->chunkTreeSearch(chunkRoot,
                [this, logicalAddr, &chunk](const LeafNode* leaf)
                { return this->findChunkItem(leaf, logicalAddr, chunk); });
        
        return chunk;
    }
}

