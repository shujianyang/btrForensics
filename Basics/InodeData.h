//! \file
//! \author Shujian Yang
//!
//! Header file of class InodeData

#ifndef INODE_DATA_H
#define INODE_DATA_H

#include <ctime>
#include <iostream>
#include <string>
#include <tsk/libtsk.h>

namespace btrForensics{
    //! Inode item data.
    class InodeData {
    private:
        uint64_t generation;
        uint64_t transId;

        uint64_t stSize; //0x10
        uint64_t stBlocks;
        
        uint64_t blockGroup; //0x20
        uint32_t stNlink;
        uint32_t stUid;
        uint32_t stGid;
        uint32_t stMode;
        uint64_t stRdev;

        uint8_t flags[8]; //0x40
        uint64_t sequence;

    public:
        time_t accessTime; //!< Acess time.
        time_t createdTime; //!< Inode info change time.
        time_t modifiedTime; //!< Modification time.

    public:
        InodeData(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~InodeData() = default; //!< Destructor

        //! Get size of the file.
        uint64_t getSize() const { return stSize; }

        std::string dataInfo() const ;
        
        static const int SIZE_OF_INODE_DATA= 0xa0; //!< Size of an inode in bytes.
    };
}

#endif

