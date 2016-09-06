//! \file
//! \author Shujian Yang
//!
//! Header file of class ChunkTree

#include <iostream>
#include "ChunkTree.h"
#include "Functions.h"

namespace btrForensics {
    ChunkTree::ChunkTree(const SuperBlock* superBlk, const TreeExaminer* treeExaminer)
            :examiner(treeExaminer)
    {
        uint64_t chunkTreePhyAddr = 
            getChunkAddr(superBlk->getChunkTrRootAddr(), &superBlk->chunkKey, &superBlk->chunkData);
        //std::cout << "---------------------" << chunkTreePhyAddr << std::endl;

        char* diskArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        tsk_img_read(examiner->image, chunkTreePhyAddr, diskArr, BtrfsHeader::SIZE_OF_HEADER);
        BtrfsHeader *chunkHeader = new BtrfsHeader(examiner->endian, (uint8_t*)diskArr);
        delete [] diskArr;

        uint64_t itemListStart = chunkTreePhyAddr + BtrfsHeader::SIZE_OF_HEADER;

        const BtrfsNode* chunkTree;
        if(chunkHeader->isLeafNode())
            chunkRoot = new LeafNode(examiner->image, chunkHeader, examiner->endian, itemListStart);
        else
            chunkRoot = new InternalNode(examiner->image, chunkHeader, TSK_LIT_ENDIAN, itemListStart);
    }


    uint64_t ChunkTree::getPhysicalAddr(uint64_t logicalAddr)
    {
        uint64_t physicalAddr(0);
        //std::cout << chunkRoot->info() << std::endl;
        examiner->leafSearchById(chunkRoot, logicalAddr,
                [&physicalAddr](const LeafNode* leaf, uint64_t targetLogAddr)
                { return getPhyAddr(leaf, targetLogAddr, physicalAddr); });
        
        return physicalAddr;
    }
}

