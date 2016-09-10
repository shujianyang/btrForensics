//! \file
//! \author Shujian Yang
//!
//! Implementation of class DirContent.

#include <sstream>
#include <iomanip>
#include "DirContent.h"

using namespace std;

namespace btrForensics {
    //! Constructor of DirContent.
    DirContent::DirContent(const InodeItem* inodeItem,
            const InodeRef* inodeRef, std::vector<const BtrfsItem*> &dirItems)
        :name(inodeRef->getDirName()), inode(inodeItem), ref(inodeRef)
    {
        for(auto item : dirItems){
            DirItem* dirEntry = (DirItem*)item;
            children.push_back(dirEntry);
        }

        if(name == "..")  // This is the root directory.
            name = "/";
    }


    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream& os, const DirContent& dirc)
    {
        os << "[" << dirc.name << "]" << endl;
        for(auto child : dirc.children) {
            if(child->getTargetType() == ItemType::ROOT_ITEM)
                continue;

            os << "  \e(0\x74\x71\e(B" << dec;
            os << setfill(' ') << setw(9) << child->getTargetInode();

            if(child->type == DirItemType::DIRECTORY)
                os << "  [" << child->getDirName() << "]\n";
            else
                os << "  " << child->getDirName() << "\n";
        }
        os << endl;
    }

}

