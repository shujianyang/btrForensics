//! \file
//! \author Shujian Yang
//!
//! Implementation of class FileTreeAnalyzer.

#include <map>
#include <sstream>
#include <functional>
#include <vector>
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
    //!
    const void FileTreeAnalyzer::listDirItems(ostream &os) const
    {
        //leafTraverse(fileTreeRoot, bind(printLeafDir, _1, _2, ref(os)));

        //Choose Lamba over std::bind.
        //See "Effective Modern C++" Item 34.
        leafTraverse(fileTreeRoot,
                [&os](const LeafNode *leaf) { printLeafDir(leaf, os); });
    }


    //! Locate the directory with give inode number.
    //!
    //! \param Inode number of directory.
    //!
    DirContent* FileTreeAnalyzer::getDirConent(uint64_t id) const
    {
        const BtrfsItem* foundItem;
        if(leafSearch(fileTreeRoot,
                [&foundItem, id](const LeafNode* leaf)
                { return findItem(leaf, id, 1, foundItem); })) {
            InodeItem* rootInode = (InodeItem*)foundItem;

            leafSearch(fileTreeRoot,
                [&foundItem, id](const LeafNode* leaf)
                { return findItem(leaf, id, 0xc, foundItem); });
            InodeRef* rootRef = (InodeRef*)foundItem;

            vector<BtrfsItem*> foundItems;
            leafSearch(fileTreeRoot,
                [&foundItems, id](const LeafNode* leaf)
                { return findNewItem(leaf, id, 0x54, foundItems); });
            
            return new DirContent(rootInode, rootRef, foundItems);
        }
        return nullptr;
    }

    //! Locate the root directory.
    DirContent* FileTreeAnalyzer::getRootDir() const
    {
        return getDirConent(rootDirId);
    }



    const void FileTreeAnalyzer::explorFiles(std::ostream& os, istream& is) const
    {
        DirContent* dir = getRootDir();
        if(dir == nullptr) {
            os << "Root directory not found." << endl;
            return;
        }

        os << "Root directory content:" <<endl;
        os << *dir;

        while(true) {
            uint64_t targetInode;
            map<int, int> dirList;
            string input;
            int inputId;

            int count(0);
            for(int i = 0; i < dir->children.size(); ++i) {
                if(dir->children[i]->getType() == 0x2)
                    dirList[++count] = i;
            }
            if(count == 0) {
                while(true) {
                    os << "No child directory is found.\n";
                    os << "(Enter '0' to go back to previous directory or 'q' to quit.)" << endl;
                    is >> input;
                    if(input == "q") return;
                    if(input == "0") break;
                }
                targetInode = dir->ref->itemHead->key.offset;
            }
            else{
                while(true) {
                    os << "Child directory with following inode numbers are found." << endl;
                    for(auto entry : dirList) {
                        DirItem* item = dir->children[entry.second];
                        os << "[" << entry.first << "] " << item->getInodeNum() << " " << item->getDirName() << '\n';
                    }
                    os << endl;
                    os << "To visit a child directory, please enter its index in the list:\n";
                    os << "(Enter '0' to go back to previous directory or 'q' to quit.)" << endl;
                    is >> input;
                    
                    if(input == "q") return;
                    stringstream(input) >> inputId;
                    if(inputId == 0) {
                        targetInode = dir->ref->itemHead->key.offset;
                        break;
                    }
                    else if(dirList.find(inputId) != dirList.end()) {
                        int target = dirList[inputId];
                        DirItem* targetItem = dir->children[target];
                        targetInode = targetItem->getInodeNum();
                        break;
                    }
                    os << "Wrong object id, please enter a correct one.\n" << endl;
                } 
                os << endl;
            }

            DirContent* newDir = getDirConent(targetInode);
            delete dir;
            dir = newDir;

            if(newDir == nullptr) {
                os << "Error, Directory not found." << endl;
                return;
            }
            os << std::string(30, '=') << "\n\n";
            os << "Directory content:" <<endl;
            os << *dir;
        }
    }
}
