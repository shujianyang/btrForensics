//! \file
//! \author Shujian Yang
//!
//! Define useful enumerations

#ifndef ENUMS_H
#define ENUMS_H

#include <iostream>

namespace btrForensics {
    //! Type of an item.
    enum  class ItemType : uint8_t {
        INODE_ITEM = 0x01,
        INODE_REF= 0x0c,
        INODE_EXTREF= 0x0d,
        XATTR_ITEM = 0x18,
        ORPHAN_ITEM = 0x30,
        DIR_LOG_ITEM = 0x3c,
        DIR_LOG_INDEX = 0x48,
        DIR_ITEM = 0x54,
        DIR_INDEX = 0x60,
        EXTENT_DATA = 0x6c,
        EXTENT_CSUM = 0x80,
        ROOT_ITEM = 0x84,
        ROOT_BACKREF = 0x90,
        ROOT_REF = 0x9c,
        EXTENT_ITEM = 0xa8,
        TREE_BLOCK_REF = 0xb0,
        EXTENT_DATA_REF = 0xb2,
        EXTENT_REF_V0 = 0xb4,
        SHARED_BLOCK_REF = 0xb6,
        SHARED_DATA_REF = 0xb8,
        BLOCK_GROUP_ITEM = 0xc0,
        DEV_EXTENT = 0xcc,
        DEV_ITEM = 0xd8,
        CHUNK_ITEM = 0xe4,
        STRING_ITEM = 0xfd,
        UNKNOWN = 0xff
    };

    //! Type of a dir item.
    enum class DirItemType : uint8_t {
        UNKNOWN, //!< Unknown
        REGULAR_FILE, //!< Regular file
        DIRECTORY, //!< Directory
        CHAR_DEVICE, //!< Character device
        BLK_DEVICE, //!< Block device
        FIFO, //!< FIFO
        SOCKET, //!< Socket
        SYMB_LINK, //!< Symbolic link
        EXT_ATTR //!< Extended attribute
    };
}

#endif