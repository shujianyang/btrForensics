//! \file
//! \author Shujian Yang
//!
//! Implementation of class InodeItem

#include <sstream>
#include "InodeItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    //! Constructor of InodeItem.
    //!
    //! \param head Item head points to this data.
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing inode data.
    //!
    InodeItem::InodeItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsItem(head), data(endian, arr)
    {
    }


    //! Print time info.
    std::string InodeItem::printTime() const
    {
        std::stringstream oss;
        oss << "Created time:  " << std::asctime(std::localtime(&data.createdTime));
        oss << "Access time:   " << std::asctime(std::localtime(&data.accessTime));
        oss << "Modified time: " << std::asctime(std::localtime(&data.modifiedTime));
        return oss.str();
    }
    

    //! Return infomation about the item data as string.
    std::string InodeItem::dataInfo() const
    {
        return data.dataInfo();
    }

}
