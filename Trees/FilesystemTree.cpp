//! \file
//! \author Shujian Yang
//!
//! Implementation of class FilesystemTree.

#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>
#include <vector>
#include "FilesystemTree.h"
#include "Tools/Functions.h"

using namespace std;
//using namespace std::placeholders;

namespace btrForensics {
    //! Constructor of tree analyzer.
    //!
    //! \param rootNode Root node of the root tree to be analyzed.
    //! \praram rootDirId Root item id of the file system tree.
    //! \param treeExaminer Tree examiner used to analyze file system tree.
    //!
    FilesystemTree::FilesystemTree(const BtrfsNode* rootNode,
            uint64_t rootItemId, const TreeExaminer* treeExaminer)
            :examiner(treeExaminer)
    {
        const BtrfsItem* foundItem;
        RootItem* rootItm;
        if(examiner->treeSearchById(rootNode, rootItemId,
                [&foundItem](const LeafNode* leaf, uint64_t targetId)
                { return searchForItem(leaf, targetId, ItemType::ROOT_ITEM, foundItem); })) {
            rootItm = (RootItem*)foundItem;
        }
        else {
            cerr << "Error. Filesystem tree not found." << endl;
            exit(1);
        }

        uint64_t offset = rootItm->getBlockNumber();
        uint64_t physicalAddr = examiner->getPhysicalAddr(offset);
        rootDirId = rootItm->getRootObjId();

        char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        tsk_img_read(examiner->image, physicalAddr, headerArr, BtrfsHeader::SIZE_OF_HEADER);
        const BtrfsHeader *fileTreeHeader = 
            new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
        delete [] headerArr;

        uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;

        if(fileTreeHeader->isLeafNode()){
            fileTreeRoot = new LeafNode(examiner->image, fileTreeHeader, examiner->endian, itemOffset);
        }
        else {
            fileTreeRoot = new InternalNode(examiner->image, fileTreeHeader, examiner->endian, itemOffset);
        }
    }


    //!< Destructor
    FilesystemTree::~FilesystemTree()
    {
        if(fileTreeRoot != nullptr)
            delete fileTreeRoot;
    }


    //! List all dir items in this tree.
    //!
    //! \param os Output stream where the infomation is printed.
    //!
    const void FilesystemTree::listDirItems(ostream &os) const
    {
        //treeTraverse(fileTreeRoot, bind(printLeafDir, _1, _2, ref(os)));

        //Choose Lamba over std::bind.
        //See "Effective Modern C++" Item 34.
        examiner->treeTraverse(fileTreeRoot,
                [&os](const LeafNode *leaf) { printLeafDir(leaf, os); });
    }


    //! Locate the directory with give inode number.
    //!
    //! \param id Inode number of directory.
    //!
    DirContent* FilesystemTree::getDirConent(uint64_t id) const
    {
        const BtrfsItem* foundItem;
        if(examiner->treeSearchById(fileTreeRoot, id,
                [&foundItem](const LeafNode* leaf, uint64_t targetId)
                { return searchForItem(leaf, targetId, ItemType::INODE_ITEM, foundItem); })) {
            InodeItem* rootInode = (InodeItem*)foundItem;

            examiner->treeSearchById(fileTreeRoot, id,
                [&foundItem](const LeafNode* leaf, uint64_t targetId)
                { return searchForItem(leaf, targetId, ItemType::INODE_REF, foundItem); });
            InodeRef* rootRef = (InodeRef*)foundItem;

            vector<BtrfsItem*> foundItems;
            examiner->treeSearchById(fileTreeRoot, id,
                [&foundItems](const LeafNode* leaf, uint64_t targetId)
                { return filterItems(leaf, targetId, ItemType::DIR_INDEX, foundItems); });
            
            return new DirContent(rootInode, rootRef, foundItems);
        }
        return nullptr;
    }


    //! List files in a directory and navigate to subdirectory.
    //!
    //! \param rootId Start position of file expoloring.
    //! \param os Output stream where the infomation is printed.
    //! \param is Input stream telling which directory is the one to be read.
    //!
    const void FilesystemTree::explorFiles(std::ostream& os, istream& is) const
    {
        DirContent* dir = getDirConent(rootDirId);
        if(dir == nullptr) {
            os << "Root directory not found." << endl;
            return;
        }

        os << "Root directory content:\n" <<endl;
        os << *dir;

        os << "The following items are subvolumes or snapshots:" << endl;
        for(auto dirItem : dir->children) {
            if(dirItem->getTargetType() == ItemType::ROOT_ITEM) {
                ostringstream oss;
                os << "  \e(0\x74\x71\e(B" << dec;
                oss << "[" << dirItem->getTargetInode() << "]";
                os << setfill(' ') << setw(9) << oss.str();
                os << "  " << dirItem->getDirName() << '\n';
            }
        }
        os << endl;

        while(true) {
            uint64_t targetInode;
            map<int, int> dirList;
            string input;
            int inputId;

            int count(0);
            for(int i = 0; i < dir->children.size(); ++i) {
                if(dir->children[i]->type == DirItemType::DIRECTORY 
                        && dir->children[i]->getTargetType() == ItemType::INODE_ITEM)
                    dirList[++count] = i;
            }
            if(count == 0) {
                while(true) {
                    os << "No child directory is found.\n";
                    os << "(Enter 'r' to return to previous directory or 'q' to quit.)" << endl;
                    is >> input;
                    if(input == "q") return;
                    if(input == "r") break;
                }
                targetInode = dir->ref->itemHead->key.offset;
            }
            else{
                while(true) {
                    os << "Child directory with following inode numbers are found." << endl;
                    for(auto entry : dirList) {
                        DirItem* item = dir->children[entry.second];
                        os << "[" << dec << setfill(' ') << setw(2) << entry.first << "] "
                            << setw(7) << item->getTargetInode() << "   " << item->getDirName() << '\n';
                    }
                    os << endl;
                    os << "To visit a child directory, please enter its index in the list:\n";
                    os << "(Enter 'r' to return to previous directory or 'q' to quit.)" << endl;
                    is >> input;
                    
                    if(input == "q") return;
                    if(input == "r") {
                        targetInode = dir->ref->itemHead->key.offset;
                        break;
                    }
                    stringstream(input) >> inputId;
                    if(dirList.find(inputId) != dirList.end()) {
                        int target = dirList[inputId];
                        DirItem* targetItem = dir->children[target];
                        targetInode = targetItem->getTargetInode();
                        break;
                    }
                    os << "Wrong index, please enter a correct one.\n" << endl;
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
            os << std::string(60, '=') << "\n\n";
            os << "Directory content:\n" <<endl;
            os << *dir;
        }
    }
}
