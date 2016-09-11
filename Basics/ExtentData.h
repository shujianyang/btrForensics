//! \file
//! \author Shujian Yang
//!
//! Header file of class ExtentData

#ifndef EXTENT_DATA_H
#define EXTENT_DATA_H

#include <string>
#include <tsk/libtsk.h>
#include "Basics.h"

namespace btrForensics {
    //Contents of a file
    class ExtentData : public BtrfsItem {
    public:
        uint64_t dataAddress;

        uint64_t generation;
        uint64_t decodedSize;
        uint8_t compression;
        uint8_t encryption;
        uint16_t otherEncoding;
        uint8_t type;

        uint64_t logicalAddress;
        uint64_t extentSize;
        uint64_t extentOffset;
        uint64_t numOfBytes;

    public:
        ExtentData(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[], uint64_t address);
        ~ExtentData() = default; //!< Destructor

        std::string dataInfo() const override;

        static const uint64_t PART_ONE_SIZE = 0x15; //! Size of first part of extent data.
    };
}

#endif
