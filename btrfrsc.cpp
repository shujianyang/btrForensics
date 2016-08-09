/** \file btrfrsc.cpp
  * Main function.
  */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
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
    BtrfsHeader header(TSK_LIT_ENDIAN, (uint8_t*)diskArr);

    cout << header << endl;
    cout << endl;
    delete [] diskArr;

    uint64_t itemListStart = supblk.getRootTrRootAddr() + BtrfsHeader::SIZE_OF_HEADER;
    cout << "Item list start address: " << itemListStart << endl;
    uint64_t itemOffset(0);
    int numOfItems = header.getNumOfItems();

    cout << "Root tree item list:" << endl;
    cout << string(30, '=') << '\n' << endl;
    for(int i=0; i<numOfItems; i++){
        diskArr = new char[BtrfsItem::SIZE_OF_ITEM]();
        tsk_img_read(img, itemListStart + itemOffset, diskArr, BtrfsItem::SIZE_OF_ITEM);

        cout << "Item address: " << itemListStart + itemOffset << endl;

        BtrfsItem item(TSK_LIT_ENDIAN, (uint8_t*)diskArr);
        cout << item;
        cout << "Data address: " << itemListStart + item.getDataOffset() << endl;
        cout << endl;

        ItemData *itmData = nullptr;
        char *itmArr = new char[item.getDataSize()]();
        uint64_t dirOffset = itemListStart + item.getDataOffset();
        tsk_img_read(img, dirOffset, itmArr, item.getDataSize());

        if(item.key.getItemType() == 0x54){
            itmData = new DirItem(TSK_LIT_ENDIAN, (uint8_t*)itmArr);
        }
        else if(item.key.getItemType() == 0x84){
            itmData = new RootItem(TSK_LIT_ENDIAN, (uint8_t*)itmArr);
        }

        if(itmData != nullptr){
            cout << *itmData << endl;
        }

        delete [] diskArr;
        delete itmData;

        itemOffset += BtrfsItem::SIZE_OF_ITEM;

        cout << string(30, '=') << '\n' << endl;
    }

    cout << endl;

}

