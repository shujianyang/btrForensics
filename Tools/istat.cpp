//! \file istat.cpp
//! \author Shujian Yang
//!
//! Main function of istat.
//!
//! Print information about an inode.

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
#include "Pool/Pool.h"
#include "Utility/Utility.h"

using namespace std;

using namespace btrForensics;

int main(int argc, char *argv[])
{
    TSK_OFF_T offsetSector(0);
    uint64_t rootFsId(0);
    int option;
    vector<string> offsetStr;
    vector<TSK_OFF_T> devOffsets;

    while((option = getopt(argc, argv, "o:s:")) != -1){
        stringstream ss;
        switch(option){
            case 'o':
                offsetStr = strSplit(optarg, ",");
                for(auto str : offsetStr){
                    if( (offsetSector = tsk_parse_offset(str.c_str())) == -1){
                        tsk_error_print(stderr);
                        exit(1);
                    }
                    devOffsets.push_back(offsetSector);
                }
                break;
            case 's':
                ss << optarg;
                ss >> rootFsId;
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
    if(argc < optind + 2) {
        cerr << "Please provde the file inode number." << endl;
        exit(1);
    }

    if(devOffsets.size() == 0)
        devOffsets.push_back(0);

    string img_name(argv[optind]);
    
    TSK_IMG_INFO *img = tsk_img_open(1, &argv[optind], TSK_IMG_TYPE_DETECT, 0);
    if(img == NULL){
        tsk_error_print(stderr);
        cerr << "Cannot open image " << img_name << "." << endl;
        exit(1);
    }

    for(auto &offSec : devOffsets) {
        TSK_OFF_T offsetByte(offSec * img->sector_size);
        if( offsetByte >= img->size){
            cerr << "Offset is too large." << endl;
            exit(1);
        }
        offSec = offsetByte;
    }

    try {
        BtrfsPool btr(img, TSK_LIT_ENDIAN, devOffsets, rootFsId);

        uint64_t targetId(btr.fsTree->rootDirId);
        stringstream ss;
        ss << argv[optind+1];
        ss >> targetId;

        bool success;
        success = btr.fsTree->showInodeInfo(targetId, cout);

        if(!success)
            cout << "Error: Inode not found." << endl;
    } catch(std::bad_alloc& ba) {
        cerr << "Error when allocating objects.\n" << ba.what() << endl;
    } catch(FsDamagedException& fsEx) {
        cerr << "Error: Btrfs filesystem damaged.\n" << fsEx.what() << endl;
    } catch(exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}

