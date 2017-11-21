//! \file
//! \author Shujian Yang
//!
//! Implementation of class BtrfsExaminer.

#include <iostream>
#include "BtrfsExaminer.h"

using namespace std;

namespace btrForensics {

    //! Constructor
    BtrfsExaminer::BtrfsExaminer(TSK_IMG_INFO *img, vector<TSK_OFF_T> devOffsets)
    {
        uint64_t devCount(0);
        for(auto dev_off : devOffsets) {
            char *diskArr = new char[SuperBlock::SUPBLK_SIZE]();
            tsk_img_read(img, dev_off + SuperBlock::SUPBLK_ADDR, 
                    diskArr, SuperBlock::SUPBLK_SIZE);
            SuperBlock supblk(TSK_LIT_ENDIAN, (uint8_t*)diskArr);

            if(fsUUID.isUnused()) {
                fsUUID = supblk.fsUUID;
            }
            else if(fsUUID != supblk.fsUUID) {
                throw FsUuidNotMatchException("Found superblocks do not belong to the same pool.");
            }

            const DevData *dev = &(supblk.devItemData);
            deviceTable[dev->deviceId] = new DeviceRecord(dev->deviceId,
                                            dev_off, dev->devUUID);

            devCount = supblk.numDevices;
        }

        if(devOffsets.size() == devCount) {
            cout << "All devices accepted. Device number: " << devCount << std::endl;
            cout << "Pool UUID: " << fsUUID.encode() << endl;
            cout << "-----------------------------------" << endl;

            for(const auto dev : deviceTable) {
                cout << (dev.second)->devInfo() << endl;
            }
        }
        else
            std::cout << "Input incomplete: " << devCount - devOffsets.size()
                << " device(s) missing." << std::endl;
    }
}

