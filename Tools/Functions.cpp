//! \file
//! \author Shujian Yang
//!
//! Implementations of functions declared in Functions.h.

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
            if(item->getItemType() == 0x54){
                DirItem *dir = (DirItem*)item;
                if(dir->getType() == 0x2)
                    os << "[Directory] ";
                os << dir->getDirName() << '\n';
            }
        }
        os << '\n';
    }


    //! Search for an item with given inode number in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param inodeNum The inode number to search for.
    //! \param type The type of the item to search for.
    //! \param item Found ItemHead pointer.
    //!
    //! \return True if the item is found.
    //!
    bool searchForItem(const LeafNode* leaf, uint64_t inodeNum,
           uint8_t type, const BtrfsItem* &foundItem)
    {
        for(auto item : leaf->itemList) {
            if(item->getId() == inodeNum &&
                    item->getItemType() == type) {
                foundItem = item;
                return true;
            }
        }
        return false;
    }

}

