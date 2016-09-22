//! \file
//! \author Shujian Yang
//!
//! Implementation of class TreeExaminer.

#include <map>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <functional>
#include "TreeExaminer.h"
#include "Functions.h"

using namespace std;
//using namespace std::placeholders;

namespace btrForensics {
    //! Constructor of tree analyzer.
    //!
    //! \param img Image file.
    //! \param end The endianess of the array.
    //! \param superBlk Pointer to btrfs super block.
    //!
    TreeExaminer::TreeExaminer(TSK_IMG_INFO *img, TSK_ENDIAN_ENUM end,
            const SuperBlock* superBlk)
        :image(img), endian(end)
    {
        initializeRootTree(superBlk);

        uint64_t defaultId = getDefaultFsId();

        fsTree = new FilesystemTree(rootTree, defaultId, this);
        fsTreeDefault = fsTree;
    }


    //! Constructor of tree analyzer, use subvolume instead of default filesystem root.
    //!
    //! \param img Image file.
    //! \param end The endianess of the array.
    //! \param superBlk Pointer to btrfs super block.
    //! \param fsRootId Id of root of Filesystem Tree.
    //!
    TreeExaminer::TreeExaminer(TSK_IMG_INFO *img, TSK_ENDIAN_ENUM end,
            const SuperBlock* superBlk, uint64_t fsRootId)
        :image(img), endian(end)
    {
        initializeRootTree(superBlk);

        const BtrfsItem* foundItem;
        if(treeSearchById(rootTree, fsRootId,
                [&foundItem](const LeafNode* leaf, uint64_t targetId)
                { return searchForItem(leaf, targetId, ItemType::ROOT_BACKREF, foundItem); })) {
            //const RootRef* dir = static_cast<const RootRef*>(foundItem);
            fsTree = new FilesystemTree(rootTree, fsRootId, this);
            fsTreeDefault = fsTree;
        }
        else
            throw runtime_error("The argument of -s option is not a valid subvolume id.");

    }


    //! Destructor
    TreeExaminer::~TreeExaminer()
    {
        if(chunkTree != nullptr)
            delete chunkTree;
        if(fsTreeDefault != fsTree && fsTree != nullptr)
            delete fsTree;
        if(fsTreeDefault != nullptr)
            delete fsTreeDefault;
        if(rootTree != nullptr)
            delete rootTree;
    }


    //! Convert logical address to physical address.
    //!
    //! \param logicalAddr 64-bit logial address.
    //! \return 64-bit physical address.
    //!
    uint64_t TreeExaminer::getPhysicalAddr(uint64_t logicalAddr) const
    {
        return chunkTree->getPhysicalAddr(logicalAddr);
    }


    //! Initialize root of Root Tree.
    void TreeExaminer::initializeRootTree(const SuperBlock* superBlk)
    {
        //Chunk tree is needed at the very beginning
        //to convert logical address to physical address.
        chunkTree = new ChunkTree(superBlk, this);

        uint64_t rootTreelogAddr = superBlk->getRootLogAddr();

        //Pyhsical address of root of Root Tree obtained here.
        uint64_t rootTreePhyAddr = chunkTree->getPhysicalAddr(rootTreelogAddr);

        char* diskArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        tsk_img_read(image, rootTreePhyAddr, diskArr, BtrfsHeader::SIZE_OF_HEADER);
        BtrfsHeader* rootHeader = new BtrfsHeader(endian, (uint8_t*)diskArr);
        delete [] diskArr;

        uint64_t itemListStart = rootTreePhyAddr + BtrfsHeader::SIZE_OF_HEADER;
        if(rootHeader->isLeafNode())
            rootTree = new LeafNode(image, rootHeader, endian, itemListStart);
        else
            rootTree = new InternalNode(image, rootHeader, endian, itemListStart);
    }


    //! Obatin root item id of default volume (filesystem tree).
    uint64_t TreeExaminer::getDefaultFsId() const
    {
        //Technically, the default volume id should be found in offset 0x80 in superblock.
        //Currently, it is set as 6 by default.
        uint64_t defaultDirId(6);

        uint64_t defaultId(0);
        const BtrfsItem* foundItem;
        if(treeSearchById(rootTree, defaultDirId,
                [&foundItem](const LeafNode* leaf, uint64_t targetId)
                { return searchForItem(leaf, targetId, ItemType::DIR_ITEM, foundItem); })) {
            const DirItem* dir = static_cast<const DirItem*>(foundItem);
            defaultId = dir->targetKey.objId; //This is id of root item to filesystem tree.
        }
        else 
            throw FsDamagedException("Default directory dir item not found.");

        return defaultId;
    }


    //! Navigate to selected node and print information.
    //!
    //! \param root Starting root node.
    //! \param os Output stream where the infomation is printed.
    //! \param is Input stream telling which node is the one to be read.
    //!
    const void TreeExaminer::navigateNodes(const BtrfsNode* root, ostream& os, istream& is) const
    {
        const BtrfsNode *node = root;
        const BtrfsHeader *header;
        while(true) {
            header = node->nodeHeader;
            os << node->info() << endl;

            uint64_t offset(0);
            map<uint64_t, uint64_t> nodeAddrs;
            if(header->isLeafNode()){
                const LeafNode *leaf = static_cast<const LeafNode*>(node);

                for(auto item : leaf->itemList){
                    if(item->getItemType() == ItemType::ROOT_ITEM){
                        const RootItem *rootItm = static_cast<const RootItem*>(item);
                        nodeAddrs[item->getId()]
                            = rootItm->getBlockNumber();
                    }
                }
            }
            else {
                const InternalNode* internal = static_cast<const InternalNode*>(node);

                for(auto ptr : internal->keyPointers) {
                    nodeAddrs[ptr->key.objId] = ptr->getBlkNum();
                }
            }

            if(nodeAddrs.size() == 0) {
                os << "This is a leaf node with no root items." << endl;
                break;
            }

            bool quit(false);
            string input;
            uint64_t inputId;
            while(true) {
                os << "Child nodes or tree roots with following object ids are found." << endl;
                for(auto addr : nodeAddrs) {
                    if(header->isLeafNode())
                        os << dec << addr.first << " ";
                    else
                        os << dec << "[" << addr.first << "] ";
                }
                    
                os << endl;
                os << "To read a child node or a tree root, please enter its object id in the list: ";
                os << "(Enter 'q' to quit.)" << endl;
                is >> input;
                
                if(input == "q") return;

                stringstream(input) >> inputId;
                if(nodeAddrs.find(inputId) != nodeAddrs.end()) break;
                os << "Wrong object id, please enter a correct one.\n" << endl;
            }
            os << endl;

            bool nodeExisted(false);
            using ConstNodePtr = const BtrfsNode*;
            ConstNodePtr* childPtr(nullptr);
            if(!header->isLeafNode()) {
                const InternalNode* internal = static_cast<const InternalNode*>(node);

                for(auto ptr : internal->keyPointers) {
                    if(ptr->key.objId == inputId && ptr->childNode != nullptr) {
                        node = ptr->childNode;
                        nodeExisted = true;
                        break;
                    }
                    else if(ptr->key.objId == inputId) {
                        childPtr = &ptr->childNode;
                        break;
                    }
                }
            }
            if(nodeExisted) continue; //The child node has already been built.

            offset = nodeAddrs[inputId];
            uint64_t physicalAddr = getPhysicalAddr(offset);

            char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
            tsk_img_read(image, physicalAddr, headerArr, BtrfsHeader::SIZE_OF_HEADER);
            header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
            delete [] headerArr;

            uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;

            if(header->isLeafNode()){
                node = new LeafNode(image, header, endian, itemOffset);
            }
            else {
                node = new InternalNode(image, header, endian, itemOffset);
            }

            if(childPtr!=nullptr)
                *childPtr = node;
        }
    }


    //! Switch to a subvolume or snapshot and exploere files within.
    //!
    //! \param os Output stream where the infomation is printed.
    //! \param is Input stream telling which node is the one to be read.
    //! \return True if switched.
    //!
    const bool TreeExaminer::switchFsTrees(ostream& os, istream& is)
    {
        vector<const BtrfsItem*> foundRootRefs;
        treeTraverse(rootTree, [&foundRootRefs](const LeafNode* leaf)
                { return filterItems(leaf, ItemType::ROOT_BACKREF, foundRootRefs); });
        
        if(foundRootRefs.size() == 0) {
            os << "\nNo subvolumes or snapshots are found.\n" << endl;
            return false;
        }

        uint64_t selectedId(0);
        while(true) {
            os << "The following subvolumes or snapshots are found:" << endl;
            int index(0);
            for(auto item : foundRootRefs) {
                const RootRef* ref = static_cast<const RootRef*>(item);
                os << "[" << dec << setfill(' ') << setw(2) << ++index << "] "
                    << setw(7) << ref->getId() << "   " << ref->getDirName() << '\n';
            }
            os << endl;

            string input;
            os << "To visit a subvolume or snapshot, please enter its index in the list:\n";
            os << "(Enter ''q' to quit.)" << endl;
            is >> input;

            if(input == "q") return false;
            int inputIndex;
            stringstream(input) >> inputIndex;
            if(inputIndex > 0 && inputIndex <= foundRootRefs.size()) {
                selectedId = foundRootRefs[inputIndex-1]->getId();
                break;
            }
            os << "Wrong index, please enter a correct one.\n\n\n" << endl;
        }
        
        fsTree = new FilesystemTree(rootTree, selectedId, this);
        os << "\n" << std::string(60, '=') << "\n";
        os << endl;
        return true;
    }


    //! Recursively traverse child nodes and process it if it is a leaf node.
    //!
    //! \param node Node being processed.
    //! \param readOnlyFunc A function type which accepts a LeafNode* 
    //!        and a vector<uint64_t>& parameters and returns void.
    //!
    void TreeExaminer::treeTraverse(const BtrfsNode *node,
            function<void(const LeafNode*)> readOnlyFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            const LeafNode* leaf = static_cast<const LeafNode*>(node);
            readOnlyFunc(leaf);
        }
        else {
            const InternalNode* internal = static_cast<const InternalNode*>(node);
            const BtrfsNode *newNode;

            for(auto ptr : internal->keyPointers) {
                if(ptr->childNode != nullptr) {
                    newNode = ptr->childNode;
                }
                else {
                    uint64_t physicalAddr = getPhysicalAddr(ptr->getBlkNum());
                    char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                    tsk_img_read(image, physicalAddr, headerArr, BtrfsHeader::SIZE_OF_HEADER);
                    BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                    delete [] headerArr; 

                    uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;

                    if(header->isLeafNode()){
                        newNode = new LeafNode(image, header, endian, itemOffset);
                    }
                    else {
                        newNode = new InternalNode(image, header, endian, itemOffset);
                    }

                    ptr->childNode = newNode;
                }

                if(newNode != nullptr)
                    treeTraverse(newNode, readOnlyFunc);
            }
        }
    }


    //! Recursively traverse child nodes and search if it is a leaf node.
    //!
    //! \param node Node being processed.
    //! \param searchFunc A function type which accepts a LeafNode* 
    //!        parameter and returns true if certain object is found.
    //! \return True if target is found in leaf node.
    //!
    bool TreeExaminer::treeSearch(const BtrfsNode *node,
            function<bool(const LeafNode*)> searchFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            const LeafNode *leaf = static_cast<const LeafNode*>(node);
            return searchFunc(leaf);
        }
        else {
            const InternalNode *internal = static_cast<const InternalNode*>(node);
            const BtrfsNode *newNode;

            for(auto ptr : internal->keyPointers) {
                if(ptr->childNode != nullptr) {
                    newNode = ptr->childNode;
                }
                else {
                    uint64_t physicalAddr = getPhysicalAddr(ptr->getBlkNum());
                    char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                    tsk_img_read(image, physicalAddr, headerArr, BtrfsHeader::SIZE_OF_HEADER);
                    BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                    delete [] headerArr; 

                    uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;
                    
                    if(header->isLeafNode()){
                        newNode = new LeafNode(image, header, endian, itemOffset);
                    }
                    else {
                        newNode = new InternalNode(image, header, endian, itemOffset);
                    }

                    ptr->childNode = newNode;
                }

                if(newNode != nullptr && treeSearch(newNode, searchFunc))
                    return true;
            }
            return false;
        }
    }


    //! Recursively traverse child nodes and search if it is a leaf node.
    //!
    //! \param node Node being processed.
    //! \param targetId Object id of target to search for.
    //! \param searchFunc A function type which accepts a LeafNode* 
    //!        parameter and returns true if certain object is found.
    //! \return True if target is found in leaf node.
    //!
    bool TreeExaminer::treeSearchById(const BtrfsNode *node, uint64_t targetId,
            function<bool(const LeafNode*, uint64_t)> searchFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            const LeafNode *leaf = static_cast<const LeafNode*>(node);
            return searchFunc(leaf, targetId);
        }
        else {
            const InternalNode *internal = static_cast<const InternalNode*>(node);
            const BtrfsNode *newNode;

            const auto &vecPtr = internal->keyPointers;
            for(int i=0; i<vecPtr.size();++i) {
                auto ptr = vecPtr[i];
                if(ptr->key.objId > targetId)
                    return false;
                if(ptr->key.objId<targetId && i!=vecPtr.size()-1
                        && vecPtr[i+1]->key.objId<targetId)
                    continue;

                if(ptr->childNode != nullptr) {
                    newNode = ptr->childNode;
                }
                else {
                    uint64_t physicalAddr = getPhysicalAddr(ptr->getBlkNum());
                    char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                    tsk_img_read(image, physicalAddr, headerArr, BtrfsHeader::SIZE_OF_HEADER);
                    BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                    delete [] headerArr; 

                    uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;
                    
                    if(header->isLeafNode()){
                        newNode = new LeafNode(image, header, endian, itemOffset);
                    }
                    else {
                        newNode = new InternalNode(image, header, endian, itemOffset);
                    }

                    ptr->childNode = newNode;
                }

                if(newNode != nullptr && treeSearchById(newNode, targetId, searchFunc))
                    return true;
            }
            return false;
        }
    }
}
