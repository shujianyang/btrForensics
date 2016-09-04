//! \file
//! \author Shujian Yang
//!
//! Implementation of class FileTreeAnalyzer.

#include <map>
#include <sstream>
#include <functional>
#include "FileTreeAnalyzer.h"
#include "Functions.h"

using namespace std;
//using namespace std::placeholders;

namespace btrForensics {
    //! Constructor of tree analyzer.
    //!
    //! \param img Image file.
    //! \param rootNode Root node of the tree to be analyzed.
    //! \param end The endianess of the array.
    FileTreeAnalyzer::FileTreeAnalyzer(TSK_IMG_INFO *img,
            const LeafNode *rootNode, TSK_ENDIAN_ENUM end)
        :TreeAnalyzer(img, rootNode, end)
    {
        uint64_t offset(0);
        bool foundFSTree(false);
        RootItem *rootItm;
  
        for(auto item : root->itemList){
            if(item->getItemType() == 0x84 && item->getId() == 5){
                rootItm = (RootItem*)item;
                foundFSTree = true;
                break;
            }
        }
        if(!foundFSTree) {
            cerr << "Error. Filesystem tree not found." << endl;
            exit(1);
        }

        offset = rootItm->getBlockNumber();
        rootDirId = rootItm->rootObjId;

        char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        tsk_img_read(image, offset, headerArr, BtrfsHeader::SIZE_OF_HEADER);
        const BtrfsHeader *fileTreeHeader = 
            new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
        delete [] headerArr;

        uint64_t itemOffset = offset + BtrfsHeader::SIZE_OF_HEADER;

        if(fileTreeHeader->isLeafNode()){
            fileTreeRoot = new LeafNode(image, fileTreeHeader, TSK_LIT_ENDIAN, itemOffset);
        }
        else {
            fileTreeRoot = new InternalNode(image, fileTreeHeader, TSK_LIT_ENDIAN, itemOffset);
        }
    }


    //! List all dir items in this tree.
    //!
    //! \param os Output stream where the infomation is printed.
    const void FileTreeAnalyzer::listDirItems(ostream &os) const
    {
        //leafRecursion(fileTreeRoot, bind(printLeafDir, _1, _2, ref(os)));

        //Choose Lamba over std::bind.
        //See "Effective Modern C++" Item 34.
        leafRecursion(fileTreeRoot,
                [&os](const LeafNode *leaf) { printLeafDir(leaf, os); });
    }


    //! Retrieve the root directory.
    /*DirContent* FileTreeAnalyzer::getRootDirConent() const
    {
        BtrfsItem* foundItem;
        if(leafSearch(fileTreeRoot,
                [&foundItem](const LeafNode* leaf)
                { return searchForItem(leaf, rootDirId, 1, foundItem); })) {
        }
    }*/

}
