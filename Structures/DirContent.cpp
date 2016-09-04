//! \file
//! \author Shujian Yang
//!
//! Implementation of class DirContent.

#include "DirContent.h"

namespace btrForensics {
    //! Constructor of DirContent.
    DirContent::DirContent(InodeItem* inodeItem, InodeRef* inodeRef,
            std::vector<BtrfsItem*> &dirItems)
        :inode(inodeItem), ref(inodeRef)
    {
        for(auto item : dirItems){
            DirItem* dirEntry = (DirItem*)item;
            children.push_back(dirEntry);
        }
    }


    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream& os, const DirContent& dirc)
    {
        for(auto child : dirc.children) {
            os << "  \e(0\x74\x71\e(B";
            if(child->getType() == 0x2)
                os << "[" << child->getInodeNum() << "] ";
            else
                os << " " << child->getInodeNum() << "  ";
            os << child->getDirName() << '\n';
        }
        os << std::endl;
    }

}

