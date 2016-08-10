/** \file btrfrsc.cpp
  * Main function.
  */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <unistd.h>
#include <tsk/libtsk.h>
#include <unistd.h>
#include "btrfrsc.h"

using namespace std;

using namespace btrForensics;

int main(int argc, char *argv[])
{
    TSK_OFF_T imgOffset(0);
    int option;
    while((option = getopt(argc, argv, "o:")) != -1){
        switch(option){
            case 'o':
                if( (imgOffset = tsk_parse_offset(optarg)) == -1){
                    tsk_error_print(stderr);
                    exit(1);
                }
                break;
            case '?':
            default:
                cerr << "Unkown arguments." << endl;
        }
    }

    if(optind >= argc) {
        cerr << "Please provide the image name" << endl;
        exit(1);
    }

    string img_name(argv[optind]);
    
    TSK_IMG_INFO *img = tsk_img_open(1, &argv[optind], TSK_IMG_TYPE_DETECT, 0);
    if(img == NULL){
        tsk_error_print(stderr);
        cerr << "Cannot open image " << img_name << "." << endl;
        exit(1);
    }

    if( imgOffset * img->sector_size >= img->size){
        cerr << "Offset is too large." << endl;
        exit(1);
    }

    char *diskArr = new char[SuperBlock::SIZE_OF_SPR_BLK]();
    if(diskArr == 0){
        cerr << "Fail to allocate superblock space." << endl;
        exit(1);
    }

    tsk_img_read(img, SuperBlock::ADDR_OF_SPR_BLK, diskArr, SuperBlock::SIZE_OF_SPR_BLK);
    SuperBlock supblk(TSK_LIT_ENDIAN, (uint8_t*)diskArr);

    cout << supblk << endl;
    cout << endl;

    cout << supblk.printMagic() << endl;

    cout << supblk.printSpace() << endl;
    cout << endl;

    cout << "Label: " << supblk.printLabel() << endl;

    cout << "\n\n" << endl;
    delete [] diskArr;

    cout << "Root tree info:" << endl;

    diskArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
    tsk_img_read(img, supblk.getRootTrRootAddr(), diskArr, BtrfsHeader::SIZE_OF_HEADER);
    BtrfsHeader *rootHeader = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)diskArr);
    delete [] diskArr;

    uint64_t itemListStart = supblk.getRootTrRootAddr() + BtrfsHeader::SIZE_OF_HEADER;
    //cout << "Item list start address: " << itemListStart << endl;

    LeafNode *rootTree = new LeafNode(img, rootHeader, TSK_LIT_ENDIAN, itemListStart);

    BtrfsHeader *header = rootHeader;
    BtrfsNode *node = rootTree;
    while(true){
        bool quit(false);
        cout << node->info() << endl;

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
            cout << "This is a leaf node with no root items." << endl;
            break;
        }

        string input;
        uint64_t inOffset;
        do{
            cout << "----Child nodes with following object ids are found." << endl;
            for(auto addr : nodeAddrs)
                cout << addr.first << " ";
            cout << endl;
            cout << "To read a child node, please enter its object id in the list: ";
            cout << "(Enter 'q' to quit.)" << endl;
            cin >> input;
            
            quit = (input == "q");
            if(quit) break;

            stringstream(input) >> inOffset;
            if(nodeAddrs.find(inOffset) != nodeAddrs.end()) break;
            cout << "Wrong object id, please enter a correct one.\n" << endl;
        } while(true);

        if(quit) break;
        cout << endl;

        offset = nodeAddrs[inOffset];

        char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        tsk_img_read(img, offset, headerArr, BtrfsHeader::SIZE_OF_HEADER);
        header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
        delete [] headerArr;

        uint64_t itemOffset = offset + BtrfsHeader::SIZE_OF_HEADER;

        delete node;

        if(header->isLeafNode()){
            node = new LeafNode(img, header, TSK_LIT_ENDIAN, itemOffset);
        }
        else {
            node = new InternalNode(img, header, TSK_LIT_ENDIAN, itemOffset);
        }

    }
    cout << endl;

    string answer;
    cout << "Do you want to list all directory items in filesystem tree? (y/n)" << endl;
    cin >> answer;

    if(answer == "y") {
        uint64_t offset(0);
        map<uint64_t, uint64_t> nodeAddrs;
  
        for(auto group : rootTree->itemGroups){
            if(group->getItemType() == 0x84){
                RootItem *rootItm = (RootItem*)(group->data);
                nodeAddrs[group->item->key.objId]
                    = rootItm->getBlockNumber();
            }
        }
        if(nodeAddrs.find(5) == nodeAddrs.end()) {
            cout << "Error. Filesystem tree not found." << endl;
            return 1;
        }
        offset = nodeAddrs[5];

        char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        tsk_img_read(img, offset, headerArr, BtrfsHeader::SIZE_OF_HEADER);
        BtrfsHeader *fileTreeHeader = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
        delete [] headerArr;

        uint64_t itemOffset = offset + BtrfsHeader::SIZE_OF_HEADER;

        BtrfsNode *fileTreeRoot;
        if(fileTreeHeader->isLeafNode()){
            fileTreeRoot = new LeafNode(img, fileTreeHeader, TSK_LIT_ENDIAN, itemOffset);
        }
        else {
            fileTreeRoot = new InternalNode(img, fileTreeHeader, TSK_LIT_ENDIAN, itemOffset);
        }

        TreeAnalyzer ana(img, fileTreeRoot, TSK_LIT_ENDIAN);
        ana.listDirItems(cout);
    }

    cout << endl;

}

