/** \file
 * Implementation of class TreeAnalyzer.
 */

#include <map>
#include <iostream>
#include <sstream>
#include "TreeAnalyzer.h"

using namespace std;

namespace btrForensics {
    /**
     * Constructor of tree analyzer.
     *
     * \param img Image file.
     * \param rootNode Root node of the tree to be analyzed.
     * \param end The endianess of the array.
     *
     */
    TreeAnalyzer::TreeAnalyzer(TSK_IMG_INFO *img,
            const LeafNode *rootNode, TSK_ENDIAN_ENUM end)
        :image(img), root(rootNode), endian(end)
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


    /**
     * Navigate to selected node and print information.
     *
     */
    const void TreeAnalyzer::navigateNodes(ostream &os, istream &is) const
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

            if(inputId == 0x05) {
                if(node != root) delete node;
                node = fileTreeRoot;
                continue;
            }

            offset = nodeAddrs[inputId];

            if(node != root && node != fileTreeRoot) delete node;

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


    /**
     * List all dir items in this tree using resursion(depth first).
     *
     */
    const void TreeAnalyzer::recursiveListDir(const BtrfsNode *node,
            ostream &os, vector<uint64_t> &idTrace) const
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;
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
                os << '\n' << endl;
            }
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

                recursiveListDir(newNode, os, idTrace);
                idTrace.pop_back();
            }
        }
    }


    /** List all dir items in this tree. */
    const void TreeAnalyzer::listDirItems(ostream &os) const
    {
        vector<uint64_t> trace;
        recursiveListDir(fileTreeRoot, os, trace);
        //leafRecursion(fileTreeRoot, printLeafDir);
    }


    void TreeAnalyzer::leafRecursion(const BtrfsNode *node, void(*process)(const LeafNode*))
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;
            process(leaf);
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

                leafRecursion(newNode, process);
            }
        }
    }


    void TreeAnalyzer::printLeafDir(const LeafNode *leaf)
    {
            bool foundDir(false);

            for(auto group : leaf->itemGroups){
                if(group->getItemType() == 0x54){
                    foundDir = true;
                    DirItem *dir = (DirItem*)(group->data);
                    cout << dir->getDirName() << '\n';
                }
            }
    }
}
