//! \file
//! \author Shujian Yang
//!
//! Implementation of class BtrfsExaminer.

#include <iostream>
#include "BtrfsExaminer.h"

namespace btrForensics {

    //! Constructor
    BtrfsExaminer::BtrfsExaminer(TSK_IMG_INFO *img, vector<TSK_OFF_T> partOffsets)
    {
        for(auto partition : partOffsets) {
            char *diskArr = new char[SuperBlock::SUPBLK_SIZE]();
            tsk_img_read(img, partition + SuperBlock::SUPBLK_ADDR, 
                    diskArr, SuperBlock::SUPBLK_SIZE);
            SuperBlock supblk(TSK_LIT_ENDIAN, (uint8_t*)diskArr);

            if(fsUUID.isUnused()) {
                fsUUID = supblk.fsUUID;
            }
            else if(fsUUID != supblk.fsUUID) {
                throw fsUuidNotMatchException("Found superblocks do not belong to the same pool.");
            }
        }
        std::cout << "All devices accepted. Device number: " << partOffsets.size() << std::endl;
    }
}

