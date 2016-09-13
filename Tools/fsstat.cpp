//! \file fsstat.cpp
//! \author Shujian Yang
//!
//! Main function of fsstat.
//!
//! Print information about the file system.

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
        cerr << "Please provide the image name." << endl;
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
    delete [] diskArr;

    if(supblk.printMagic() == "_BHRfS_M") {
        cout << supblk << endl;
        cout << supblk.printSpace() << endl;
        cout << endl;
        cout << "Label: " << supblk.printLabel() << endl;
    }
    else
        cout << "Error: Superblock not found." << endl;
}

