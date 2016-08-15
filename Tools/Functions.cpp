/**
 * \file
 * \author Shujian Yang
 *
 * Implementations of functions declared in Functions.h.
 *
 */

#include "Functions.h"

namespace btrForensics{
    /**
     * Prints names of directory items stored in a leaf node.
     *
     * \param leaf Pointer to the leaf node.
     * \param idTrace The vector used to trace node ids on the path from root to node.
     * \param os Output stream where the infomation is printed.
     *
     */
    void printLeafDir(const LeafNode* leaf,
                      vector<uint64_t> &idTrace,
                      std::ostream &os)
    {
        bool foundDir(false);

        for(auto group : leaf->itemGroups){
            if(group->getItemType() == 0x54){
                foundDir = true;
                DirItem *dir = (DirItem*)(group->data);
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

}

