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

        for(uint16_t i=0; i<numStripe; ++i){
            btrStripes.push_back(new Stripe(endian, arr + arIndex));
            arIndex += Stripe::SIZE_OF_STRIPE;
        }
    }


    //! Destructor
    ChunkData::~ChunkData()
    {
        for(auto stripe : btrStripes) {
            delete stripe;
        }
    }

    //! Return infomation about the item data as string.
    std::string ChunkData::dataInfo() const
    {
        std::ostringstream oss;
        oss << "----------------------------------------------" << '\n';
        oss << "Chunk size: " << std::dec << chunkSize << '\n';
        oss << "Stripe length: " << std::dec << stripeLength << '\n';
        oss << "Number of stripes: " << numStripe << "\n";
        oss << "Sub stripes: " << subStripe << "\n\n";
        for(auto stripe : btrStripes) {
            oss << "Device ID: " << stripe->deviceId << '\n';
            oss << "Offset: 0x" << std::uppercase << std::hex  << stripe->offset << '\n';
            oss << "Device UUID: " << stripe->devUUID.encode() << '\n';
        }
        oss << "----------------------------------------------" << '\n';
        return oss.str();
    }

}
