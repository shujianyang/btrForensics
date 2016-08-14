/**
 * \file
 * \author Shujian Yang
 *
 * Implementations of functions declared in Functions.h.
 *
 */

#include <iostream>
#include "Functions.h"

namespace btrForensics{

    void printLeafDir(const LeafNode* leaf)
    {
        bool foundDir(false);

        for(auto group : leaf->itemGroups){
            if(group->getItemType() == 0x54){
                foundDir = true;
                DirItem *dir = (DirItem*)(group->data);
                std::cout << dir->getDirName() << '\n';
            }
        }

        if(foundDir){
            std::cout << std::endl;
        }
    }

}

