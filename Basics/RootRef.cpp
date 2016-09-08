//! \file
//! \author Shujian Yang
//!
//! Implementation of class RootRef

#include <sstream>
#include "RootRef.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of root ref item.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing root ref item data.
    //!
    RootRef::RootRef(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head)
    {
        int arIndex(0); //Key initialized already.
        dirId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        index = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        nameSize = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        
        dirName = new char[nameSize]();
        for(int i=0; i < nameSize; ++i){
            dirName[i] = arr[arIndex++];
        }
    }


    //!< Destructor
    RootRef::~RootRef()
    {
        if(dirName != nullptr)
            delete [] dirName;
    }


    //! Return name of the directory.
    std::string RootRef::getDirName() const
    {
        return std::string(dirName, nameSize);
    }


    //! Return infomation about the item data as string.
    std::string RootRef::dataInfo() const
    {
        std::ostringstream oss;
        oss << std::dec;
        oss << "Directory id: " << dirId << '\n';
        oss << "Directory index: " << index << '\n';
        oss << "Name: " << getDirName();
        return oss.str();
    }
}

