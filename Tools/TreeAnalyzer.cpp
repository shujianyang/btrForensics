//! \file
//! \author Shujian Yang
//!
//! Implementation of class TreeAnalyzer.

#include <map>
#include <sstream>
#include <functional>
#include "TreeAnalyzer.h"
#include "Functions.h"

using namespace std;
//using namespace std::placeholders;

namespace btrForensics {
    //! Constructor of tree analyzer.
    //!
    //! \param img Image file.
    //! \param rootNode Root node of the tree to be analyzed.
    //! \param end The endianess of the array.
    //!
    TreeAnalyzer::TreeAnalyzer(TSK_IMG_INFO *img,
            const BtrfsNode* rootNode, TSK_ENDIAN_ENUM end)
        :image(img), root(rootNode), endian(end) {}


    //! Navigate to selected node and print information.
    //!
    //! \param os Output stream where the infomation is printed.
    //! \param is Input stream telling which node is the one to be read.
    //!
    const void TreeAnalyzer::navigateNodes(ostream& os, istream& is) const
    {
        const BtrfsNode *node = root;
        const BtrfsHeader *header;
        while(true) {
            header = node->nodeHeader;
            os << node->info() << endl;

            uint64_t offset(0);
            map<uint64_t, uint64_t> nodeAddrs;
            if(header->isLeafNode()){
                LeafNode *leaf = (LeafNode*)node;

                for(auto item : leaf->itemList){
                    if(item->getItemType() == ItemType::ROOT_ITEM){
                        RootItem *rootItm = (RootItem*)item;
                        nodeAddrs[item->getId()]
                            = rootItm->getBlockNumber();
                    }
                }
            }
            else {
                InternalNode *internal = (InternalNode*)node;

                for(auto ptr : internal->keyPointers) {
                    nodeAddrs[ptr.key.objId] = ptr.getBlkNum();
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
                os << "----Child nodes with following object ids are found." << endl;
                for(auto addr : nodeAddrs)
                    os << addr.first << " ";
                os << endl;
                os << "To read a child node, please enter its object id in the list: ";
                os << "(Enter 'q' to quit.)" << endl;
                is >> input;
                
                if(input == "q") return;

                stringstream(input) >> inputId;
                if(nodeAddrs.find(inputId) != nodeAddrs.end()) break;
                os << "Wrong object id, please enter a correct one.\n" << endl;
            }
            os << endl;

            offset = nodeAddrs[inputId];

            if(node != root) delete node;

            char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
            tsk_img_read(image, offset, headerArr, BtrfsHeader::SIZE_OF_HEADER);
            header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
            delete [] headerArr;

            uint64_t itemOffset = offset + BtrfsHeader::SIZE_OF_HEADER;

            if(header->isLeafNode()){
                node = new LeafNode(image, header, TSK_LIT_ENDIAN, itemOffset);
            }
            else {
                node = new InternalNode(image, header, TSK_LIT_ENDIAN, itemOffset);
            }
        }
    }


    //! Recursively traverse child nodes and process it if it is a leaf node.
    //!
    //! \param node Node being processed.
    //! \param readOnlyFunc A function type which accepts a LeafNode* 
    //!        and a vector<uint64_t>& parameters and returns void.
    //!
    void TreeAnalyzer::leafTraverse(const BtrfsNode *node,
            function<void(const LeafNode*)> readOnlyFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;
            readOnlyFunc(leaf);
        }
        else {
            InternalNode *internal = (InternalNode*)node;
            BtrfsNode *newNode;

            for(auto &ptr : internal->keyPointers) {
                if(ptr.childNode != nullptr) {
                    newNode = ptr.childNode;
                }
                else {
                    char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                    tsk_img_read(image, ptr.getBlkNum(), headerArr, BtrfsHeader::SIZE_OF_HEADER);
                    BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                    delete [] headerArr; 

                    uint64_t itemOffset = ptr.getBlkNum() + BtrfsHeader::SIZE_OF_HEADER;

                    if(header->isLeafNode()){
                        newNode = new LeafNode(image, header, endian, itemOffset);
                    }
                    else {
                        newNode = new InternalNode(image, header, endian, itemOffset);
                    }

                    ptr.childNode = newNode;
                }

                if(newNode != nullptr)
                    leafTraverse(newNode, readOnlyFunc);
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
    bool TreeAnalyzer::leafSearch(const BtrfsNode *node,
            function<bool(const LeafNode*)> searchFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;
            return searchFunc(leaf);
        }
        else {
            InternalNode *internal = (InternalNode*)node;
            BtrfsNode *newNode;

            for(auto &ptr : internal->keyPointers) {
                if(ptr.childNode != nullptr) {
                    newNode = ptr.childNode;
                }
                else {
                    char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                    tsk_img_read(image, ptr.getBlkNum(), headerArr, BtrfsHeader::SIZE_OF_HEADER);
                    BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                    delete [] headerArr; 

                    uint64_t itemOffset = ptr.getBlkNum() + BtrfsHeader::SIZE_OF_HEADER;
                    
                    if(header->isLeafNode()){
                        newNode = new LeafNode(image, header, endian, itemOffset);
                    }
                    else {
                        newNode = new InternalNode(image, header, endian, itemOffset);
                    }

                    ptr.childNode = newNode;
                }

                if(newNode != nullptr && leafSearch(newNode, searchFunc))
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
    bool TreeAnalyzer::leafSearchById(const BtrfsNode *node, uint64_t targetId,
            function<bool(const LeafNode*, uint64_t)> searchFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;
            return searchFunc(leaf, targetId);
        }
        else {
            InternalNode *internal = (InternalNode*)node;
            BtrfsNode *newNode;

            for(auto &ptr : internal->keyPointers) {
                if(ptr.key.objId > targetId)
                    return false;
                if(ptr.childNode != nullptr) {
                    newNode = ptr.childNode;
                }
                else {
                    char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                    tsk_img_read(image, ptr.getBlkNum(), headerArr, BtrfsHeader::SIZE_OF_HEADER);
                    BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                    delete [] headerArr; 

                    uint64_t itemOffset = ptr.getBlkNum() + BtrfsHeader::SIZE_OF_HEADER;
                    
                    if(header->isLeafNode()){
                        newNode = new LeafNode(image, header, endian, itemOffset);
                    }
                    else {
                        newNode = new InternalNode(image, header, endian, itemOffset);
                    }

                    ptr.childNode = newNode;
                }

                if(newNode != nullptr && leafSearchById(newNode, targetId, searchFunc))
                    return true;
            }
            return false;
        }
    }
}
