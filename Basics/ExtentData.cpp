//! \file
//! \author Shujian Yang
//!
//! Header file of class ExtentData

#include <sstream>
#include "ExtentData.h"

namespace btrForensics {
    //! Constructor of extent data.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing extent data.
    //! \param address Physical address of current Btrfs item data part.
    //!
    ExtentData::ExtentData(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[], uint64_t address)
        :BtrfsItem(head)
    {
        int arIndex(0);

        generation = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        decodedSize = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        compression = arr[arIndex++];
        encryption = arr[arIndex++];

        otherEncoding = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        type = arr[arIndex++];

        if(type == 0) {
            dataAddress = address + PART_ONE_SIZE;
        }
        else {
            logicalAddress = read64Bit(endian, arr + arIndex);
            arIndex += 0x08;
            extentSize = read64Bit(endian, arr + arIndex);
            arIndex += 0x08;
            extentOffset = read64Bit(endian, arr + arIndex);
            arIndex += 0x08;
            numOfBytes = read64Bit(endian, arr + arIndex);
            arIndex += 0x08;
        }
    }


    //! Return infomation about the item data as string.
    std::string ExtentData::dataInfo() const
    {
        std::ostringstream oss;
        oss << "Size: " << std::dec << decodedSize << " bytes\n";
        oss << "Compression: " << (int)compression << "\n";
        oss << "Type: " << (type==0 ? "inline" : "regular") << "\n";
        
        if(type != 0) {
            oss << std::uppercase << std::hex;
            oss << "Logical address of extent: " << logicalAddress << "\n";
            oss << "Size of extent: " << extentSize << "\n";
            oss << "Offset within extent: " << extentOffset << "\n";
            oss << "Logical number of bytes: " << numOfBytes << "\n";
        }

        return oss.str();
    }
}