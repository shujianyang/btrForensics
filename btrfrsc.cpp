/**
 * \file btrfrsc.cpp
 * \author Shujian Yang
 *
 * Main function.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <memory>
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

    diskArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
    tsk_img_read(img, supblk.getRootTrRootAddr(), diskArr, BtrfsHeader::SIZE_OF_HEADER);
    BtrfsHeader *rootHeader = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)diskArr);
    delete [] diskArr;

    uint64_t itemListStart = supblk.getRootTrRootAddr() + BtrfsHeader::SIZE_OF_HEADER;

    const LeafNode *rootTree = new LeafNode(img, rootHeader, TSK_LIT_ENDIAN, itemListStart);
    //Root tree root built.

    string answer;
    
    do {
        cout << "MAIN MENU -- What do you want to do?" << endl;
        cout << "[1] Navigate to selected node and print information." << endl;
        cout << "[2] List all directory items in filesystem tree." << endl;
        cout << "[q] Quit." << endl;
        cout << "Enter your choice: ";
        cin >> answer;

        cout << endl;
        if(answer == "1"){
            TreeAnalyzer navigator(img, rootTree, TSK_LIT_ENDIAN);
            navigator.navigateNodes(cout, cin);
        }
        else if(answer == "2") {
            cout << "Listing directory items...\n" << endl;
            TreeAnalyzer list(img, rootTree, TSK_LIT_ENDIAN);
            list.listDirItems(cout);
        }
        else if(answer == "q") break;
        else
            cout << "Invalid option. Please choose again." << endl;

        cout << endl;
    } while(true);

    cout << endl;

}

