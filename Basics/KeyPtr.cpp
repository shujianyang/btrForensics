//! \file
//! \author Shujian Yang
//!
//! Implementation of class KeyPtr

#include "KeyPtr.h"
#include "Utility/ReadInt.h"
#include "Trees/BtrfsNode.h"

namespace btrForensics{

    //! Constructor of Btrfs key pointer.
    //!
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing key pointer data.
    //!
    KeyPtr::KeyPtr(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :key(endian, arr), childNode(nullptr)
    {
        int arIndex(BtrfsKey::SIZE_OF_KEY); //Key initialized already.
        blkNum = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        generation = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
    }


    //!< Destructor
    KeyPtr::~KeyPtr()
    {
        if(childNode != nullptr)
            delete childNode;
    }


    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream &os, const KeyPtr &keyPtr)
    {
        os << keyPtr.key;
        os << "Block number: " << keyPtr.blkNum << '\n';
        os << "Generation: " << keyPtr.generation << '\n';

        return os;
    }

}
