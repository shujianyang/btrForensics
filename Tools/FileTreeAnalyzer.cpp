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
        map<uint64_t, uint64_t> nodeAddrs;
  
        for(auto group : root->itemGroups){
            if(group->getItemType() == 0x84){
                RootItem *rootItm = (RootItem*)(group->data);
                nodeAddrs[group->item->key.objId]
                    = rootItm->getBlockNumber();
            }
        }
        if(nodeAddrs.find(5) == nodeAddrs.end()) {
            cerr << "Error. Filesystem tree not found." << endl;
            exit(1);
        }
        offset = nodeAddrs[5];

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
        vector<uint64_t> trace{0x05};

        //leafRecursion(fileTreeRoot, trace, bind(printLeafDir, _1, _2, ref(os)));

        //Choose Lamba over std::bind.
        //See "Effective Modern C++" Item 34.
        leafRecursion(fileTreeRoot, trace,
                [&os](const LeafNode *leaf, vector<uint64_t> &idTrace)
                { printLeafDir(leaf, idTrace, os); });
    }

}
