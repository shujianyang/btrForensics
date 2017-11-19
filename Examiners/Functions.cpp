//! \file
//! \author Shujian Yang
//!
//! Implementations of functions declared in Functions.h.

#include <algorithm>
#include "Functions.h"

namespace btrForensics{
    //! Prints names of files stored in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param os Output stream where the infomation is printed.
    //!
    void printLeafDir(const LeafNode* leaf, std::ostream &os)
    {
        for(auto item : leaf->itemList){
            if(item->getItemType() == ItemType::DIR_INDEX){
                const DirItem *dir = static_cast<const DirItem*>(item);
                if(dir->type == DirItemType::REGULAR_FILE)
                    os << dir->getDirName() << '\n';
            }
        }
    }


    //! Search for first item with given id and type in a leaf node.
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
            if(item->getId() > inodeNum) //Items are sorted in leaf nodes by ids.
                return false;
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
           vector<const BtrfsItem*> &vec)
    {
        for(auto item : leaf->itemList) {
            if(item->getId() > inodeNum) //Items are sorted in leaf nodes by ids.
                return true;
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


    //! Find all items with given type in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param type The type of the item to search for.
    //! \param vec Vector storing found items.
    //!
    void filterItems(const LeafNode* leaf, ItemType type, vector<const BtrfsItem*> &vec)
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
    //! \return Return true when the address is acquired.
    //!
    /*bool getPhyAddr(const LeafNode* leaf, uint64_t targetLogAddr,
           uint64_t& targetPhyAddr)
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

        //Matched chunk may not be found.
        //Then physical address equals to logical address?(Not 100% sure.)
        if(target == nullptr)
            targetPhyAddr = targetLogAddr;

        const ChunkItem* chunk = static_cast<const ChunkItem*>(target);
        //Read the chunk to calculate actual physical address.
        targetPhyAddr = 
            getChunkAddr(targetLogAddr, &chunk->itemHead->key, &chunk->data);
        
        return true;
    }*/


    //! Caculate the physical address by comparing givel logical address
    //! with (logical, physical) address pair stored in a chunk item.
    //!
    //! \param logicalAddr Logical address to convert.
    //! \param key Key storing logical address.
    //! \param chunkData Chunk item data storing corresponding physical address.
    //!
    //! \return Mapped physical address. 0 if not valid.
    //!
    /*uint64_t getChunkAddr(uint64_t logicalAddr,
            const BtrfsKey* key, const ChunkData* chunkData)
    {
        uint64_t physicalAddr;
        uint64_t chunkLogical = key->offset; //Key offset stores logical address.
        uint64_t chunkPhysical = chunkData->getOffset(); //Data offset stores physical address.

        //Input logical address should be larger than chunk logial address.
        if(logicalAddr < chunkLogical)
            throw FsDamagedException("Superblock chunk item error. Unable to map logical address to physical address.");
        
        physicalAddr = logicalAddr - chunkLogical + chunkPhysical;
        return physicalAddr;
    }*/


    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream& os, const DirItemType& type)
    {
        switch(type) {
            case DirItemType::UNKNOWN:
                os << "~";
                break;
            case DirItemType::REGULAR_FILE:
                os << "r";
                break;
            case DirItemType::DIRECTORY:
                os << "d";
                break;
            case DirItemType::CHAR_DEVICE:
                os << "c";
                break;
            case DirItemType::BLK_DEVICE:
                os << "b";
                break;
            case DirItemType::FIFO:
                os << "p";
                break;
            case DirItemType::SOCKET:
                os << "h";
                break;
            case DirItemType::SYMB_LINK:
                os << "l";
                break;
            case DirItemType::EXT_ATTR:
                os << "e";
                break;
        };

        return os;
    }
}

