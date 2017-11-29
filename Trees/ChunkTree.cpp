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
        uint64_t chunkTreePhyAddr = btrPool->readData(diskArr, supBlk->chunkTrRootAddr,
                &(supBlk->chunkKey), &(supBlk->chunkData), BtrfsHeader::SIZE_OF_HEADER);

        BtrfsHeader *chunkHeader = new BtrfsHeader(pool->endian, (uint8_t*)diskArr);
        delete [] diskArr;

        //std::cout << *chunkHeader << std::endl;
        uint64_t itemListStart = chunkTreePhyAddr + BtrfsHeader::SIZE_OF_HEADER;

        const BtrfsNode* chunkTree;
        if(chunkHeader->isLeafNode())
            chunkRoot = new LeafNode(pool->image, chunkHeader,
                                pool->endian, itemListStart);
        else
            chunkRoot = new InternalNode(pool->image, chunkHeader,
                                TSK_LIT_ENDIAN, itemListStart);
    }


    //!< Destructor
    ChunkTree::~ChunkTree()
    {
        if(chunkRoot != nullptr)
            delete chunkRoot;
    }


    //! Convert logical address to physical address.
    //!
    //! \param logicalAddr 64-bit logial address.
    //! \return 64-bit physical address.
    //!
    uint64_t ChunkTree::getPhysicalAddr(uint64_t logicalAddr)
    {
        uint64_t physicalAddr(0);
        //std::cout << chunkRoot->info() << std::endl;
        btrPool->treeSearch(chunkRoot,
                [this, logicalAddr, &physicalAddr](const LeafNode* leaf)
                { return this->btrPool->getPhyAddrFromChunkTree(leaf, logicalAddr, physicalAddr); });
        
        return physicalAddr;
    }
}

