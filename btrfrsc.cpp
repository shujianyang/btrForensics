//! \file btrfrsc.cpp
//! \author Shujian Yang
//!
//! Main function.

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
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
    int option;
    string offsetArgs;
    vector<TSK_OFF_T> devOffsets;

    while((option = getopt(argc, argv, "o:")) != -1){
        switch(option){
            case 'o':
                offsetArgs = optarg;
                if( (offsetSector = tsk_parse_offset(optarg)) == -1){
                    tsk_error_print(stderr);
                    exit(1);
                }
                devOffsets.push_back(offsetSector);
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
        BtrfsPool btr(img, TSK_LIT_ENDIAN, devOffsets);

        SuperBlock *supblk = btr.primarySupblk;

        cout << "Magic: " << supblk->printMagic() << endl;

        cout << supblk->printSpace() << endl;
        cout << endl;

        cout << "Label: " << supblk->printLabel() << endl;
        cout << "\n" << endl;

        string answer;
        
        while(true) {
            cout << "MAIN MENU -- What do you want to do?" << endl;
            cout << "[1] Browse nodes derived from root tree and print information." << endl;
            cout << "[2] Browse nodes in chunk tree and print information." << endl;
            cout << "[3] Browse nodes in filesystem tree and print information." << endl;
            cout << "[4] List all files in default filesystem tree." << endl;
            cout << "[5] Explor files and subdirectories in default root directory." << endl;
            cout << "[6] Switch to a subvolume or snapshot and explore files within." << endl;
            cout << "[7] Read a file from image and save to current directory." << endl;
            cout << "[q] Quit." << endl;
            cout << "Enter your choice > ";
            cin >> answer;
            cout << endl;

            if(answer == "q") break;
            cout << std::string(60, '=') << "\n";
            cout << endl;
            if(answer == "1"){
                btr.navigateNodes(btr.rootTree, cout, cin);
            }
            else if(answer == "2"){
                btr.navigateNodes(btr.chunkTree->chunkRoot, cout, cin);
            }
            else if(answer == "3"){
                btr.navigateNodes(btr.fsTreeDefault->fileTreeRoot, cout, cin);
            }
            else if(answer == "4") {
                cout << "Listing directory items...\n" << endl;
                uint64_t targetId(btr.fsTree->rootDirId);
                btr.fsTree->listDirItemsById(targetId, true, true, true, 0, cout);
                cout << endl;
            }
            /*else if(answer == "5") {
                examiner.fsTree->explorFiles(cout, cin);
            }
            else if(answer == "6") {
                if(examiner.switchFsTrees(cout, cin)) {
                    examiner.fsTree->explorFiles(cout, cin);
                    delete examiner.fsTree;
                    examiner.fsTree = examiner.fsTreeDefault;
                }
            }
            else if(answer == "7") {
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
                    cout << "Success: File written to current directory." << endl;
                else
                    cout << "Error: File not found or has no content." << endl;
            }*/
            else
                cout << "Invalid option. Please choose again." << endl;

            cout << endl;
        }
    } catch(std::bad_alloc& ba) {
        cerr << "Error when allocating objects.\n" << ba.what() << endl;
    } catch(FsDamagedException& fsEx) {
        cerr << "Error: Btrfs filesystem damaged.\n" << fsEx.what() << endl;
    } catch(exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}

