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
    void printLeafDir(const LeafNode* leaf,
                      vector<uint64_t> &idTrace,
                      std::ostream &os)
    {
        bool foundDir(false);

        for(auto group : leaf->itemGroups){
            if(group->getItemType() == 0x54){
                foundDir = true;
                DirItem *dir = (DirItem*)(group->data);
                if(dir->getType() == 0x2)
                    os << "[Directory] ";
                os << dir->getDirName() << '\n';
            }
        }

        if(foundDir){
            os << "Node id --- ";
            for(auto id: idTrace){
                os << "[" << id << "] ";
            }
            os << '\n' << std::endl;
        }
    }


    //! Search for an item with given inode number in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param inodeNum The inode number to search for.
    //! \param type The type of the item to search for.
    //! \param item Found BtrfsItem pointer.
    //!
    //! \return True if the item is found.
    //!
    bool searchForItem(const LeafNode* leaf, uint64_t inodeNum,
           uint8_t type, const BtrfsItem* &item)
    {
        for(auto group : leaf->itemGroups) {
            if(group->getId() == inodeNum &&
                    group->getItemType() == type) {
                item = group->item;
                return true;
            }
        }
        return false;
    }

}

