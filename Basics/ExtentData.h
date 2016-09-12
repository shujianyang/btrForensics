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
        uint64_t dataAddress; //!< Physical address of inline data, only works for inline file.

        uint64_t generation; //!< Generaion.
        uint64_t decodedSize; //!< Decoded size of extent.
        uint8_t compression; //!< Compression algorithm, 0 for none.
        uint8_t encryption; //!< Encryption algorithm, 0 for none.
        uint16_t otherEncoding; //!< 0 for none.
        uint8_t type; //!< 0 if inline.

        uint64_t logicalAddress; //!< Logical address of extent for non-inline file.
        uint64_t extentSize; //!< Size of extent for non-inline file.
        uint64_t extentOffset; //!< Offset within extent for non-inline file.
        uint64_t numOfBytes; //!< Logical number of bytes in extent for non-inline file.

    public:
        ExtentData(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[], uint64_t address);
        ~ExtentData() = default; //!< Destructor

        std::string dataInfo() const override;

        static const uint64_t PART_ONE_SIZE = 0x15; //!< Size of first part of extent data.
    };
}

#endif
