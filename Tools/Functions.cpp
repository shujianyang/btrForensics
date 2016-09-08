//! \file
//! \author Shujian Yang
//!
//! Implementations of functions declared in Functions.h.

#include <algorithm>
#include "Functions.h"

namespace btrForensics{
    //! Prints names of directory items stored in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param idTrace The vector used to trace node ids on the path from root to node.
    //! \param os Output stream where the infomation is printed.
    //!
    void printLeafDir(const LeafNode* leaf, std::ostream &os)
    {
        for(auto item : leaf->itemList){
            if(item->getItemType() == ItemType::DIR_INDEX){
                DirItem *dir = (DirItem*)item;
                if(dir->type == DirItemType::REGULAR_FILE)
                    os << dir->getDirName() << '\n';
            }
        }
    }


    //! Search for an item with given id in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param inodeNum The inode number to search for.
    //! \param type The type of the item to search for.
    //! \param item Found ItemHead pointer.
    //!
    //! \return True if the item is found.
    //!
    bool searchForItem(const LeafNode* leaf, uint64_t inodeNum,
           ItemType type, const BtrfsItem* &foundItem)
    {
        for(auto item : leaf->itemList) {
            if(item->getId() > inodeNum) return false;
            if(item->getId() == inodeNum &&
                    item->getItemType() == type) {
                foundItem = item;
                return true;
            }
        }
        return false;
    }


    //! Find all items with given id and type in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param inodeNum The inode number to search for.
    //! \param type The type of the item to search for.
    //! \param vec Vector storing found items.
    //!
    //! \return True if all items with the inodeNum has been found.
    //!
    bool filterItems(const LeafNode* leaf, uint64_t inodeNum, ItemType type,
           vector<BtrfsItem*> &vec)
    {
        for(auto item : leaf->itemList) {
            if(item->getId() > inodeNum) return true;
            if(item->getId() == inodeNum &&
                    item->getItemType() == type) {
                // Is it possible to find duplicate items?
                //auto result = find(vec.cbegin(), vec.cend(), item);
                //if(result == vec.cend())
                    vec.push_back(item);
            }
        }
        return false;
    }


    //! Find all items with given id and type in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param type The type of the item to search for.
    //! \param vec Vector storing found items.
    //!
    void filterItems(const LeafNode* leaf, ItemType type, vector<BtrfsItem*> &vec)
    {
        for(auto item : leaf->itemList) {
            if(item->getItemType() == type)
                vec.push_back(item);
        }
    }


    //! Get the physical address by comparing givel logical address with chunk items in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param targetLogAddr Logical address to convert.
    //! \param targetPhyAddr Converted physical address.
    //!
    //! \return Always return true.
    //!
    bool getPhyAddr(const LeafNode* leaf, uint64_t targetLogAddr,
           uint64_t& targetPhyAddr)
    {
        BtrfsItem* target(nullptr);

        for(auto item : leaf->itemList) {
            if(item->getItemType() != ItemType::CHUNK_ITEM)
                continue;
            if(item->itemHead->key.offset <= targetLogAddr)
                target = item;
            else
                break;
        }

        if(target == nullptr)
            targetPhyAddr = targetLogAddr;

        ChunkItem* chunk = (ChunkItem*)target;
        targetPhyAddr = 
            getChunkAddr(targetLogAddr, &chunk->itemHead->key, &chunk->data);
        
        return true;
    }


    //! Caculate the physical address by comparing givel logical address
    //! with (logical, physical) address pair stored in a chunk item.
    //!
    //! \param logicalAddr Logical address to convert.
    //! \param key Key storing logical address.
    //! \param chunkData Chunk item data storing corresponding physical address.
    //!
    //! \return Mapped physical address. 0 if not valid.
    //!
    uint64_t getChunkAddr(uint64_t logicalAddr,
            const BtrfsKey* key, const ChunkData* chunkData)
    {
        uint64_t physicalAddr;
        uint64_t chunkLogical = key->offset;
        uint64_t chunkPhysical = chunkData->getOffset();

        if(logicalAddr < chunkLogical)
            return 0;
        
        physicalAddr = logicalAddr - chunkLogical + chunkPhysical;
        return physicalAddr;
    }

}

