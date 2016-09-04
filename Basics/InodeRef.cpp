//! \file
//! \author Shujian Yang
//!
//! Implementation of class InodeRef

#include <sstream>
#include "InodeRef.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of inode ref.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing inode ref data.
    //!
    InodeRef::InodeRef(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head)
    {
        int arIndex(0);
        indexInDir = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        nameSize = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        
        nameInDir = new char[nameSize]();
        for(int i=0; i < nameSize; ++i){
            nameInDir[i] = arr[arIndex++];
        }
    }


    //! Return name of the inode reference.
    std::string InodeRef::getDirName() const
    {
        return std::string(nameInDir, nameSize);
    }


    //! Return infomation about the item data as string.
    std::string InodeRef::dataInfo() const
    {
        std::ostringstream oss;
        oss << "Index in directory: " << indexInDir << '\n';
        oss << "Name in directory: " << getDirName();
        return oss.str();
    }
}

