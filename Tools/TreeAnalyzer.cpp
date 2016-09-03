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
            const LeafNode *rootNode, TSK_ENDIAN_ENUM end)
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

                for(auto group : leaf->itemGroups){
                    if(group->getItemType() == 0x84){
                        RootItem *rootItm = (RootItem*)(group->data);
                        nodeAddrs[group->item->key.objId]
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
            do{
                os << "----Child nodes with following object ids are found." << endl;
                for(auto addr : nodeAddrs)
                    os << addr.first << " ";
                os << endl;
                os << "To read a child node, please enter its object id in the list: ";
                os << "(Enter 'q' to quit.)" << endl;
                is >> input;
                
                quit = (input == "q");
                if(quit) break;

                stringstream(input) >> inputId;
                if(nodeAddrs.find(inputId) != nodeAddrs.end()) break;
                os << "Wrong object id, please enter a correct one.\n" << endl;
            } while(true);

            if(quit) break;
            os << endl;

            /*if(inputId == 0x05) {
                if(node != root) delete node;
                node = fileTreeRoot;
                continue;
            }*/

            offset = nodeAddrs[inputId];

            if(node != root /*&& node != fileTreeRoot*/) delete node;

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
    //! \param idTrace The vector used to trace node ids on the path from root to node.
    //! \param readOnlyFunc A function type which accepts a LeafNode* 
    //!        and a vector<uint64_t>& parameters and returns void.
    //!
    void TreeAnalyzer::leafRecursion(const BtrfsNode *node,
            std::vector<uint64_t> &idTrace,
            function<void(const LeafNode*, std::vector<uint64_t>&)> readOnlyFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;
            readOnlyFunc(leaf, idTrace);
        }
        else {
            map<uint64_t, uint64_t> nodeAddrs;
            InternalNode *internal = (InternalNode*)node;

            for(const auto &ptr : internal->keyPointers) {
                nodeAddrs.insert({ptr.key.objId, ptr.getBlkNum()});
            }

            for(const auto &addr : nodeAddrs) {

                char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                tsk_img_read(image, addr.second, headerArr, BtrfsHeader::SIZE_OF_HEADER);
                BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                delete [] headerArr; 

                uint64_t itemOffset = addr.second + BtrfsHeader::SIZE_OF_HEADER;

                idTrace.push_back(addr.first);
                BtrfsNode *newNode;
                if(header->isLeafNode()){
                    newNode = new LeafNode(image, header, endian, itemOffset);
                }
                else {
                    newNode = new InternalNode(image, header, endian, itemOffset);
                }

                leafRecursion(newNode, idTrace, readOnlyFunc);
                idTrace.pop_back();
            }
        }
    }


    //! Recursively traverse child nodes and search if it is a leaf node.
    //!
    //! \param node Node being processed.
    //! \param searchFunc A function type which accepts a LeafNode* 
    //!        parameter and returns true if certain object is found.
    //!
    bool TreeAnalyzer::leafSearch(const BtrfsNode *node,
            function<bool(const LeafNode*)> searchFunc) const
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;
            return searchFunc(leaf);
        }
        else {
            map<uint64_t, uint64_t> nodeAddrs;
            InternalNode *internal = (InternalNode*)node;

            for(const auto &ptr : internal->keyPointers) {
                nodeAddrs.insert({ptr.key.objId, ptr.getBlkNum()});
            }

            for(const auto &addr : nodeAddrs) {

                char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                tsk_img_read(image, addr.second, headerArr, BtrfsHeader::SIZE_OF_HEADER);
                BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                delete [] headerArr; 

                uint64_t itemOffset = addr.second + BtrfsHeader::SIZE_OF_HEADER;

                BtrfsNode *newNode;
                if(header->isLeafNode()){
                    newNode = new LeafNode(image, header, endian, itemOffset);
                }
                else {
                    newNode = new InternalNode(image, header, endian, itemOffset);
                }

                if(leafSearch(newNode, searchFunc))
                    return true;
            }
        }
    }
}
