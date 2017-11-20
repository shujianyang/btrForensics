//! \file
//! \author Shujian Yang
//!
//! Header file of class ChunkData

#ifndef CHUNK_DATA_H
#define CHUNK_DATA_H

#include <iostream>
#include <string>
#include <vector>
#include <tsk/libtsk.h>
#include "Utility/Utility.h"
#include "Stripe.h"

using std::vector;

namespace btrForensics{
    //! Chunk item data.
    class ChunkData {
    private:
        uint64_t chunkSize;
        uint64_t objId;

        uint64_t stripeLength; //0x10
        uint64_t type;
        
        uint32_t optIoAlignment; //0x20
        uint32_t optIoWidth;
        uint32_t minIoSize;
        
        uint16_t numStripe;
        uint16_t subStripe;

        vector<Stripe*> btrStripes; //0x30

    public:
        ChunkData(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~ChunkData();

        std::string dataInfo() const ;

        //! Get offset
        //uint64_t getOffset() const { return offset; }
        
        //static const int SIZE_OF_CHUNK_DATA= 0x30; //!< Size of chunk item data in bytes.
    };
}

#endif


