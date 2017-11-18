//! \file
//! \author Shujian Yang
//!
//! Implementation of class ChunkData

#include <sstream>
#include "ChunkData.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of ChunkData.
    //!
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing inode data.
    //!
    ChunkData::ChunkData(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :deviceUUID(endian, arr + 0x40)
    {
        int arIndex(0);

        chunkSize = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
        objId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        stripeLength = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
        type = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        optIoAlignment = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;
        optIoWidth = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;
        minIoSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        numStripe = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        subStripe = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;

        deviceId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
        offset = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
    }

    //! Return infomation about the item data as string.
    std::string ChunkData::dataInfo() const
    {
        std::ostringstream oss;
        oss << "Number of stripes: " << numStripe << '\n';
        oss << "Device ID: " << deviceId << '\n';
        oss << "Offset: 0x" << std::uppercase << std::hex  << offset << '\n';
        return oss.str();
    }

}
