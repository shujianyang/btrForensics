//! \file subls.cpp
//! \author Shujian Yang
//!
//! Main function of devls.
//!
//! List devices registered in a Btrfs pool.

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
#include "Basics/Basics.h"
#include "Trees/Trees.h"
#include "Examiners/Examiners.h"
#include "Utility/Utility.h"

using namespace std;

using namespace btrForensics;

int main(int argc, char *argv[])
{
    /*TSK_OFF_T offsetSector(0);
    int option;
    while((option = getopt(argc, argv, "o:")) != -1){
        switch(option){
            case 'o':
                if( (offsetSector = tsk_parse_offset(optarg)) == -1){
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

    TSK_OFF_T offsetByte(offsetSector * img->sector_size);
    if( offsetByte >= img->size){
        cerr << "Offset is too large." << endl;
        exit(1);
    }

    try {
        char *diskArr = new char[SuperBlock::SIZE_OF_SPR_BLK]();
        tsk_img_read(img, offsetByte + SuperBlock::ADDR_OF_SPR_BLK, diskArr, SuperBlock::SIZE_OF_SPR_BLK);
        SuperBlock supblk(TSK_LIT_ENDIAN, (uint8_t*)diskArr);
        delete [] diskArr;

        TreeExaminer examiner(img, offsetByte, TSK_LIT_ENDIAN, &supblk);

        vector<const BtrfsItem*> foundDevItems;
        examiner.treeTraverse((examiner.chunkTree)->chunkRoot, [&foundDevItems](const LeafNode* leaf)
                { return filterItems(leaf, ItemType::DEV_ITEM, foundDevItems); });

        if(foundDevItems.size() == 0) {
            cout << "\nNo device is found.\n" << endl;
            return 0;
        }

        cout << "The following devices are found:\n" << endl;
        for(auto item : foundDevItems) {
            const DevItem* dev = static_cast<const DevItem*>(item);
            cout << dev->dataInfo() << endl;
        }
    } catch(std::bad_alloc& ba) {
        cerr << "Error when allocating objects.\n" << ba.what() << endl;
    } catch(FsDamagedException& fsEx) {
        cerr << "Error: Btrfs filesystem damaged.\n" << fsEx.what() << endl;
    } catch(exception& e) {
        cerr << e.what() << endl;
    }*/

    return 0;
}

