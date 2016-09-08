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
            ostringstream oss;
            os << "  \e(0\x74\x71\e(B" << dec;
            if(child->type == DirItemType::DIRECTORY)
                oss << "[" << child->getTargetInode() << "]";
            else
                oss << child->getTargetInode();
            os << setfill(' ') << setw(9) << oss.str();
            os << "  " << child->getDirName() << '\n';
        }
        os << endl;
    }

}

