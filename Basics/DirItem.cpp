//! \file
//! \author Shujian Yang
//!
//! Implementation of class DirItem

#include <sstream>
#include "DirItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of dir item.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing dir item data.
    //!
    DirItem::DirItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head), key(endian, arr)
    {
        int arIndex(BtrfsKey::SIZE_OF_KEY); //Key initialized already.
        transId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        dataSize = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        nameSize = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        
        childType = arr[arIndex++];

        dirName = new char[nameSize]();
        for(int i=0; i < nameSize; ++i){
            dirName[i] = arr[arIndex++];
        }

        switch(childType) {
            case 0:
                type = DirItemType::UNKNOWN;
                break;
            case 1:
                type = DirItemType::REGULAR_FILE;
                break;
            case 2:
                type = DirItemType::DIRECTORY;
                break;
            case 3:
                type = DirItemType::CHAR_DEVICE;
                break;
            case 4:
                type = DirItemType::BLK_DEVICE;
                break;
            case 5:
                type = DirItemType::FIFO;
                break;
            case 6:
                type = DirItemType::SOCKET;
                break;
            case 7:
                type = DirItemType::SYMB_LINK;
                break;
            case 8:
                type = DirItemType::EXT_ATTR;
                break;
        }

        //Directory data not processed yet.
    }


    //! Return name of the directory.
    std::string DirItem::getDirName() const
    {
        return std::string(dirName, nameSize);
    }


    //! Return infomation about the item data as string.
    std::string DirItem::dataInfo() const
    {
        std::ostringstream oss;
        oss << key;
        oss << "Type: " << (int)childType << '\n';
        oss << "Name: " << getDirName();
        return oss.str();
    }
}

