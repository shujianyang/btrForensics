//! \file btrfrsc.cpp
//! \author Shujian Yang
//!
//! Main function.

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

    cout << "Magic: " << supblk.printMagic() << endl;

    cout << supblk.printSpace() << endl;
    cout << endl;

    cout << "Label: " << supblk.printLabel() << endl;
    cout << "\n" << endl;
    delete [] diskArr;

    TreeExaminer examiner(img, TSK_LIT_ENDIAN, &supblk);

    string answer;
    
    while(true) {
        cout << "MAIN MENU -- What do you want to do?" << endl;
        cout << "[1] Browse nodes and print information." << endl;
        cout << "[2] Browse nodes in filesystem tree and print information." << endl;
        cout << "[3] List all files in default filesystem tree." << endl;
        cout << "[4] Explor files and subdirectories in default root directory." << endl;
        cout << "[5] Switch to a subvolume or snapshot and exploere files within." << endl;
        cout << "[6] Read a file from image and save to current directory." << endl;
        cout << "[q] Quit." << endl;
        cout << "Enter your choice > ";
        cin >> answer;
        cout << endl;

        if(answer == "q") break;
        cout << std::string(60, '=') << "\n";
        cout << endl;
        if(answer == "1"){
            examiner.navigateNodes(examiner.rootTree, cout, cin);
        }
        else if(answer == "2"){
            examiner.navigateNodes(examiner.fsTreeDefault->fileTreeRoot, cout, cin);
        }
        else if(answer == "3") {
            cout << "Listing directory items...\n" << endl;
            uint64_t targetId(examiner.fsTree->rootDirId);
            examiner.fsTree->listDirItemsById(targetId, true, true, true, 0, cout);
            cout << endl;
        }
        else if(answer == "4") {
            examiner.fsTree->explorFiles(cout, cin);
        }
        else if(answer == "5") {
            examiner.switchFsTrees(cout, cin);
        }
        else if(answer == "6") {
            cout << "Please enter the inode number of file." << endl;
            cout << "(Enter 'q' to quit.)" << endl;
            string input;
            uint64_t targetId;
            bool success(false);
            while(true) {
                cin >> input;
                if(input == "q") break;
                if(stringstream(input) >> targetId) {
                    success = examiner.fsTree->readFile(targetId);
                    break;
                }
                else {
                    cin.clear();
                    cout << "Invalid input. Please try again.\n" << endl;
                }
            }
            if(success)
                cout << "File written." << endl;
            else
                cout << "File not found." << endl;
        }
        else
            cout << "Invalid option. Please choose again." << endl;

        cout << endl;
    }

    cout << endl;

}

