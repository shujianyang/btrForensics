//! \file
//! \author Shujian Yang
//!
//! Implementations of functions declared in Functions.h.

#include <algorithm>
#include <sstream>
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

