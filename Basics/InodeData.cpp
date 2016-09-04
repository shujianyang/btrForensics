//! \file
//! \author Shujian Yang
//!
//! Implementation of class InodeData

#include <sstream>
#include "InodeData.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of InodeData.
    //!
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing inode data.
    //!
    InodeData::InodeData(TSK_ENDIAN_ENUM endian, uint8_t arr[])
    {
        int arIndex(0);

        generation = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        transId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        stSize = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        stBlocks = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        blockGroup = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        stNlink = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        stUid = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        stGid = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        stMode = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        stRdev = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        for(int i=0; i<0x08; i++){
            flags[i] = arr[arIndex++];
        }

        sequence = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
    }

    //! Return infomation about the item data as string.
    std::string InodeData::dataInfo() const
    {
        std::ostringstream oss;
        oss << "Size: 0x" << stSize;
        return oss.str();
    }

}
